#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <cstdlib> // For rand() and RAND_MAX

using namespace std;

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

// Function to find the maximum sum of a circular subarray using pointer arithmetic
int maxCircularSubarraySumPointer(const unique_ptr<int[]>& arr, int n) {
    int max_kadane = kadanePointer(arr.get(), n);

    int total_sum = 0;
    int min_ending_here = *arr.get();
    int min_so_far = *arr.get();

    for (int i = 0; i < n; i++) {
        total_sum += *(arr.get() + i);
        min_ending_here = min(*(arr.get() + i), min_ending_here + *(arr.get() + i));
        min_so_far = min(min_so_far, min_ending_here);
    }

    if (total_sum == min_so_far) {
        return max_kadane;
    }

    return max(max_kadane, total_sum - min_so_far);
}

int main() {
    const int N = 50000000;
    auto arr = make_unique<int[]>(N);

    // Fill the array with random integers
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100 - 50;
    }
    
    // Benchmarking pointer version
    auto start = chrono::high_resolution_clock::now();
    int result2 = maxCircularSubarraySumPointer(arr, N);
    auto end = chrono::high_resolution_clock::now();
    auto duration_pointer = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Max circular subarray sum (pointer): " << result2 << endl;
    return 0;
}