#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

using namespace std;

// Mutex for thread-safe access to cout
mutex cout_mutex;

// Function to find the reverse of a number in binary
int findReverse(int n) {
    int reverse_bin = 0;
    while (n > 0) {
        reverse_bin = (reverse_bin << 1) | (n & 1);
        n = n >> 1;
    }
    return reverse_bin;
}

// Function to check if the binary representation of the number is a palindrome
bool isPalindrome(int n) {
    return n == findReverse(n);
}

// Worker function to check for palindrome and print results
void worker(const vector<int>& numbers, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        bool palindrome = isPalindrome(numbers[i]);
        {
            lock_guard<mutex> lock(cout_mutex); // Protect cout
            cout << "Is binary representation of " << numbers[i] << " a palindrome? " 
                 << (palindrome ? "Yes" : "No") << endl;
        }
    }
}

int main() {
    vector<int> numbers = {3, 5, 7, 9, 10, 15, 21, 31, 32}; // Example array
    size_t num_threads = thread::hardware_concurrency(); // Get the number of available threads
    vector<thread> threads;

    // Divide work among threads
    size_t chunk_size = (numbers.size() + num_threads - 1) / num_threads; // Ceiling division

    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = min(start + chunk_size, numbers.size());
        if (start < numbers.size()) {
            threads.emplace_back(worker, ref(numbers), start, end);
        }
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}