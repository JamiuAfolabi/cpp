#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Function to find the maximum sum subarray using Kadane's algorithm
int kadane(const vector<int>& arr) {
    int max_ending_here = arr[0];
    int max_so_far = arr[0];

    for (size_t i = 1; i < arr.size(); i++) {
        max_ending_here = max(arr[i], max_ending_here + arr[i]);
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

// Function to find the maximum sum of a circular subarray
int maxCircularSubarraySum(const vector<int>& arr) {
    int n = arr.size();

    // Case 1: Find maximum sum using Kadane's algorithm (not circular)
    int max_kadane = kadane(arr);

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

int main() {
    int n;
    cout << "Enter the size of the circular array: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int result = maxCircularSubarraySum(arr);
    cout << "The maximum sum of a non-empty subarray in the circular array is: " << result << endl;

    return 0;
}