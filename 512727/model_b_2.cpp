#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;


bool is_palindrome(int n) {
    if (n == 0) {
        return true;
    }
    
    bool is_negative = n < 0;
    int abs_n = is_negative ? abs(n) : n;
    string binary_str;
    
    while (abs_n != 0) {
        binary_str = to_string(abs_n % 2) + binary_str;
        abs_n /= 2;
    }
    
    // Removing leading zeros if any
    while (!binary_str.empty() && binary_str[0] == '0') {
        binary_str.erase(binary_str.begin());
    }
    
    // Check if the binary string is a palindrome
    for (size_t i = 0; i < binary_str.size(); ++i) {
        if (binary_str[i] != binary_str[binary_str.size() - i - 1]) {
            return false;
        }
    }
    
    return true;
}

int find_palindromes_concurrent(vector<int>& nums) {
    int count = 0;
    const int NUM_THREADS = 4; // Adjust the number of threads as needed
    queue<int> tasks;
    
    mutex m;
    condition_variable cv;
    bool more_tasks = true;
    
    thread workers[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        workers[i] = thread([&]() {
            while (true) {
                unique_lock<mutex> lock(m);
                cv.wait(lock, [&]() {
                    return !tasks.empty() || !more_tasks;
                });
                
                if (!more_tasks && tasks.empty()) {
                    break;
                }
                
                int task = tasks.front();
                tasks.pop();
                lock.unlock();
                
                if (is_palindrome(task)) {
                    unique_lock<mutex> lck(m);
                    count++;
                }
            }
        });
    }
    
    // Add all the numbers to the task queue
    for (const int& num : nums) {
        tasks.push(num);
    }
    
    // Signal no more tasks are coming
    {
        unique_lock<mutex> lock(m);
        more_tasks = false;
    }
    cv.notify_all();
    
    // Join the worker threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        workers[i].join();
    }
    
    return count;
}

int main() {
    vector<int> nums = {-121, -10, 1, 7, 11, 101, 858, 9};
 
    cout << "Number of palindromes in the array: " << find_palindromes_concurrent(nums) << endl;
    return 0;
}
