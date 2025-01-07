#include <iostream>
#include <vector>
#include <cmath>
#include <ctime> // For measuring time
#include <algorithm> // For std::sort

// Function to perform Binary Search
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid; // Target found
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Target not found
}

// Function to perform Jump Search
int jumpSearch(const std::vector<int>& arr, int target) {
    int size = arr.size();
    int step = static_cast<int>(sqrt(size));
    int prev = 0;

    // Jumping the blocks
    while (arr[std::min(step, size) - 1] < target) {
        prev = step;
        step += static_cast<int>(sqrt(size));
        if (prev >= size) {
            return -1; // If we've exceeded the array size
        }
    }

    // Linear search for target in the block
    while (arr[prev] < target) {
        prev++;
        if (prev == std::min(step, size)) {
            return -1;
        }
    }

    // If found, return the index
    if (arr[prev] == target) {
        return prev;
    }

    return -1; // Not found
}

int main() {
    const int N = 10000000; // 10 million elements
    std::vector<int> arr(N);

    // Fill the array with sorted data (1 to N)
    for (int i = 0; i < N; ++i) {
        arr[i] = i + 1; // Array will contain values from 1 to 10 million
    }

    // Target to search for
    int target = N; // We'll search for the largest element, which is 10 million

    // Benchmark Binary Search
    clock_t start = clock();
    int binaryResult = binarySearch(arr, target);
    clock_t end = clock();
    double binarySearchTime = double(end - start) / CLOCKS_PER_SEC;

    // Benchmark Jump Search
    start = clock();
    int jumpResult = jumpSearch(arr, target);
    end = clock();
    double jumpSearchTime = double(end - start) / CLOCKS_PER_SEC;

    // Print results
    std::cout << "Binary Search found target at index: " << binaryResult << "\n";
    std::cout << "Binary Search Time: " << binarySearchTime << " seconds\n";

    std::cout << "Jump Search found target at index: " << jumpResult << "\n";
    std::cout << "Jump Search Time: " << jumpSearchTime << " seconds\n";

    return 0;
}