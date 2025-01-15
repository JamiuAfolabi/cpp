#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <algorithm> // For min
#include <mutex>
#include <atomic>

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

void suggestSimilarWords(const string &word, const vector<string> &dictionary) {
    const int numThreads = 4;
    vector<thread> threads;
    atomic<int> minDistance(INT_MAX);
    string closestMatch;
    mutex mtx;

    int chunkSize = dictionary.size() / numThreads;
    
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? dictionary.size() : (i + 1) * chunkSize;  // Handle dfinal chunk size
        threads.emplace_back(suggestSimilarWordsThread, word, ref(dictionary), start, end, ref(closestMatch), ref(minDistance), ref(mtx));
    }

    for (auto &t : threads) {
        t.join(); // Wait for all threads to finish
    }

    if (!closestMatch.empty()) {
        cout << "Did you mean: " << closestMatch << "?" << endl;
    } else {
        cout << "No suggestions!" << endl;
    }
}

bool isInDictionary(const string &word, const vector<string> &dictionary) {
    for (const string &dictWord : dictionary) {
        if (word == dictWord) {
            return true; // Found a match
        }
    }
    return false; // No match found
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