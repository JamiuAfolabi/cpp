#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <atomic>
#include <thread>


using namespace std;

// Calculate Levenshtein distance using the optimized approach
int levenshteinDistance(const string &s1, const string &s2) {
    int lenS1 = s1.length();
    int lenS2 = s2.length();
    vector<int> dp1(lenS2 + 1);
    vector<int> dp2(lenS2 + 1);

    for (int i = 0; i <= lenS2; ++i) {
        dp1[i] = i;
    }

    for (int i = 1; i <= lenS1; ++i) {
        dp2[0] = i;
        for (int j = 1; j <= lenS2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp2[j] = min({dp1[j] + 1,
                           dp2[j - 1] + 1,
                           dp1[j - 1] + cost});
        }
        swap(dp1, dp2);
    }
    return dp1[lenS2];
}

// Thread function to suggest similar words
void suggestSimilarWordsThread(const string &word, const vector<string> &dictionary, 
                                int start, int end, 
                                string &closestMatch, 
                                atomic<int> &minDistance, 
                                mutex &mtx) {
    for (int i = start; i < end; ++i) {
        int distance = levenshteinDistance(word, dictionary[i]);
        if (distance < minDistance && distance > 0) {
            lock_guard<mutex> lock(mtx);
            if (distance < minDistance) {
                minDistance = distance;
                closestMatch = dictionary[i];
            }
        }
    }
}

void suggestSimilarWords(const string& word, const vector<string>& dictionary) {
    std::atomic<int> minDistance(INT_MAX);
    std::atomic<string> closestMatch("");

    funcInvokerT invoker(word, dictionary, minDistance, closestMatch);

    for (const string& dictWord : dictionary) {
        invoker.enqueue(dictWord);
    }

    invoker.shutdown(); 

    if (!closestMatch.load().empty()) {
        cout << "Did you mean: " << closestMatch.load() << "?" << endl;
    } else {
        cout << "No suggestions!" << endl;
    }
}


class funcInvokerT {
    private:
        string s;
        vector<string> d;
        std::atomic<int>& minDistance;
        std::atomic<string>& closestMatch;
        
        std::thread ths_;
        std::vector<std::future<void>> futs_;

        struct job{
            const string word;
            funcInvokerT& invoker;
        };
        
        std::condition_variable  cv;
        std::mutex        mtx;
        std::queue<job>        q;
        bool shutting_down = false;


    public:
        funcInvokerT(const string& s, const vector<string>& d, std::atomic<int>& m, std::atomic<string>& c) : s(s), d(d), minDistance(m), closestMatch(c), ths_([this]() {
            for(;;){
                job j = {";}", *this};
                val_or_complete(q.dequeue(), -1);
                if(! shutting_down){
                    invoker_thread(j);
                } 
            }  
        }) {       
        }

        ~funcInvokerT() {       
            shutdown();
        }
        
    void shutdown(){ 
            std::lock_guard<std::mutex> lock(mtx);   
            shutting_down = true;            
            cv.notify_all();
            ths_.join();
        }
        
        void enqueue(const string& w){ 
            std::unique_lock<std::mutex> lock(mtx);        
            q.push(job{w, *this});
            cv.notify_one();
        }  

        void invoker_thread(job j) {
           int distance = levenshteinDistance(s, j.word);
           if (distance < minDistance.load() && distance > 0) {
                minDistance.store(distance);
                closestMatch.store(j.word);
            }
        }

        template <typename JobT>
        std::future<void> val_or_complete(JobT&& v, const int(&distance_func)(decltype(v)), std::atomic<int>& x)
        {
            const int val = distance_func(v);
            if (val < x.load() && val > 0)
            {
                x.store(val);
                closestMatch.store(std::get<0>(v));
            }
            auto r = std::make_shared<std::packaged_task<void()>>([v]() {});
            std::future<void> fut = r->get_future();
            futs_.push_back(move(fut));
            return fut;
        }


};

bool isInDictionary(const string& word, const vector<string>& dictionary) {
    for (const string& dictWord : dictionary) {
        if (word == dictWord) {
            return true;
        }
    }
    return false;
}


int main() {
    vector<string> dictionary = {"apple", "banana", "orange", "grape", "peach"};

    string word;
    cout << "Enter a word: ";
    cin >> word;

    if (isInDictionary(word, dictionary)) {
        cout << word << " is in the dictionary." << endl;
    } else {
        cout << word << " is not in the dictionary." << endl;
        suggestSimilarWords(word, dictionary);
    }

    return 0;
}