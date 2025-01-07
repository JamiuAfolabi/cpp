#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// Function to perform binary search
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Function to perform Jump Search
int jumpSearch(std::vector<int>& arr, int target) {
    int step = static_cast<int>(sqrt(arr.size()));
    int prev = 0;

    while (arr[std::min(step, arr.size() - 1)] < target) {
        prev = step;
        step += static_cast<int>(sqrt(arr.size()));
        if (prev >= arr.size()) {
            return -1;
        }
    }

    while (arr[prev] < target) {
        prev++;
        if (prev == std::min(step, arr.size())) {
            return -1;
        }
    }

    return arr[prev] == target ? prev : -1;
}

int main() {
    const int SIZE = 10'000'000;

    // Generate a sorted array
    std::vector<int> arr(SIZE);
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }

    int target = 8'000'000;  // You can choose any target

    // Binary Search benchmark
    {
        auto start = std::chrono::high_resolution_clock::now();
        int result = binarySearch(arr, target);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Binary Search execution time: " << duration.count() / 1'000'000 << " milliseconds\n";
    }

    // Jump Search benchmark
    {
        auto start = std::chrono::high_resolution_clock::now();
        int result = jumpSearch(arr, target);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Jump Search execution time: " << duration.count() / 1'000'000 << " milliseconds\n";
    }

    return 0;
}
