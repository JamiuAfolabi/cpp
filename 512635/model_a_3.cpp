#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // For rand() and RAND_MAX

using namespace std;

// Function to find the maximum sum subarray using Kadane's algorithm with array indexing
int kadaneIndexing(const vector<int>& arr) {
    int max_ending_here = arr[0];
    int max_so_far = arr[0];

    for (size_t i = 1; i < arr.size(); i++) {
        max_ending_here = max(arr[i], max_ending_here + arr[i]);
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

// Function to find the maximum sum subarray using Kadane's algorithm with pointer arithmetic
int kadanePointer(int* arr, int n) {
    int max_ending_here = *arr;
    int max_so_far = *arr;

    for (int i = 1; i < n; i++) {
        max_ending_here = max(*(arr + i), max_ending_here + *(arr + i));
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

// Function to find the maximum sum of a circular subarray using array indexing
int maxCircularSubarraySumIndexing(const vector<int>& arr) {
    int n = arr.size();
    int max_kadane = kadaneIndexing(arr);

    int total_sum = 0;
    int min_ending_here = arr[0];
    int min_so_far = arr[0];

    for (int i = 0; i < n; i++) {
        total_sum += arr[i];
        min_ending_here = min(arr[i], min_ending_here + arr[i]);
        min_so_far = min(min_so_far, min_ending_here);
    }

    if (total_sum == min_so_far) {
        return max_kadane;
    }

    return max(max_kadane, total_sum - min_so_far);
}

// Function to find the maximum sum of a circular subarray using pointer arithmetic
int maxCircularSubarraySumPointer(int* arr, int n) {
    int max_kadane = kadanePointer(arr, n);

    int total_sum = 0;
    int min_ending_here = *arr;
    int min_so_far = *arr;

    for (int i = 0; i < n; i++) {
        total_sum += *(arr + i);
        min_ending_here = min(*(arr + i), min_ending_here + *(arr + i));
        min_so_far = min(min_so_far, min_ending_here);
    }

    if (total_sum == min_so_far) {
        return max_kadane;
    }

    return max(max_kadane, total_sum - min_so_far);
}

int main() {
    const int N = 50000000; // Size of the array
    vector<int> arr(N);

    // Fill the array with random integers
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100 - 50; // Random numbers between -50 and 49
    }

    // Benchmarking indexing version
    auto start = chrono::high_resolution_clock::now();
    int result1 = maxCircularSubarraySumIndexing(arr);
    auto end = chrono::high_resolution_clock::now();
    auto duration_indexing = chrono::duration_cast<chrono::milliseconds>(end - start);

    // Benchmarking pointer version
    start = chrono::high_resolution_clock::now();
    int result2 = maxCircularSubarraySumPointer(arr.data(), N);
    end = chrono::high_resolution_clock::now();
    auto duration_pointer = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Max circular subarray sum (indexing): " << result1 << " Time taken: " << duration_indexing.count() << " ms" << endl;
    cout << "Max circular subarray sum (pointer): " << result2 << " Time taken: " << duration_pointer.count() << " ms" << endl;

    return 0;
}