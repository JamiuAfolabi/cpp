#include <iostream>
#include <vector>
#include <chrono>
#include <climits>

using namespace std;

// Function to find the maximum sum subarray using Kadane's algorithm with indexing
int kadane_indexing(const vector<int>& arr) {
    int max_ending_here = arr[0];
    int max_so_far = arr[0];

    for (size_t i = 1; i < arr.size(); i++) {
        max_ending_here = max(arr[i], max_ending_here + arr[i]);
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

// Function to find the maximum sum subarray using Kadane's algorithm with pointer
int kadane_pointer(const vector<int>& arr) {
    int n = arr.size();
    int* arr_ptr = arr.data();

    int max_ending_here = *arr_ptr;
    int max_so_far = *arr_ptr;

    for (int i = 1; i < n; i++) {
        max_ending_here = max(*(arr_ptr + i), max_ending_here + *(arr_ptr + i));
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

// Function to find the maximum sum of a circular subarray using indexing
int maxCircularSubarraySum_indexing(const vector<int>& arr) {
    int n = arr.size();
    
    // ... (Same implementation as before using indexing)
    // Case 1: Find maximum sum using Kadane's algorithm (not circular)
    int max_kadane = kadane_indexing(arr);

    // Case 2: Find the sum of the entire array and the minimum subarray sum
    int total_sum = 0;
    int min_ending_here = arr[0];
    int min_so_far = arr[0];

    for (int i = 0; i < n; i++) {
        total_sum += arr[i];
        min_ending_here = min(arr[i], min_ending_here + arr[i]);
        min_so_far = min(min_so_far, min_ending_here);
    }

    // If all numbers are negative, return the maximum sum from the first part
    if (total_sum == min_so_far) {
        return max_kadane;
    }

    // The maximum circular sum is total_sum - min_so_far
    int max_circular = total_sum - min_so_far;

    // Return the maximum of the two cases
    return max(max_kadane, max_circular);
    
}

// Function to find the maximum sum of a circular subarray using pointer
int maxCircularSubarraySum_pointer(const vector<int>& arr) {
    int n = arr.size();
    int* arr_ptr = arr.data();
    
    // ... (Same implementation as before using pointer)
    int max_kadane = kadane_pointer(arr);

    // Case 2: Find the sum of the entire array and the minimum subarray sum
    int total_sum = 0;
    int min_ending_here = *arr;
    int min_so_far = *arr;

    for (int i = 0; i < n; i++) {
        total_sum += *(arr + i);
        min_ending_here = min(*(arr + i), min_ending_here + *(arr + i));
        min_so_far = min(min_so_far, min_ending_here);
    }

    // If all numbers are negative, return the maximum sum from the first part
    if (total_sum == min_so_far) {
        return max_kadane;
    }

    // The maximum circular sum is total_sum - min_so_far
    int max_circular = total_sum - min_so_far;

    // Return the maximum of the two cases
    return max(max_kadane, max_circular);
}

int main() {
    const int size = 50'000'000; // 50 million elements
    vector<int> arr(size);

    // Generate random data for the array
    for (int& i : arr) {
        i = rand() % 1000 - 500; // Random integers between -500 and 499
    }

    /* Benchmarking */

    {
        auto start = chrono::high_resolution_clock::now();
        int result = kadane_indexing(arr);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Kadane's Algorithm (Indexing): " << duration.count() << " microseconds, Result: " << result << endl;
    }

    {
        auto start = chrono::high_resolution_clock::now();
        int result = kadane_pointer(arr);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Kadane's Algorithm (Pointer): " << duration.count() << " microseconds, Result: " << result << endl;
    }
    
    {
        auto start = chrono::high_resolution_clock::now();
        int result = maxCircularSubarraySum_indexing(arr);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Max Circular Subarray Sum (Indexing): " << duration.count() << " microseconds, Result: " << result << endl;
    }

    {
        auto start = chrono::high_resolution_clock::now();
        int result = maxCircularSubarraySum_pointer(arr);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Max Circular Subarray Sum (Pointer): " << duration.count() << " microseconds, Result: " << result << endl;
    }

    return 0;
}