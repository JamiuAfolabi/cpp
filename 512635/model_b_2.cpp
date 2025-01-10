#include <iostream>
using namespace std;

int kadane(int* arr, int n) {
    int max_ending_here = *arr;
    int max_so_far = *arr;

    for (int i = 1; i < n; i++) {
        arr++;
        max_ending_here = max(*arr, max_ending_here + *arr);
        max_so_far = max(max_so_far, max_ending_here);
    }
    return max_so_far;
}

int maxCircularSubarraySum(int* arr, int n) {

    // Case 1: Find maximum sum using Kadane's algorithm (not circular)
    int max_kadane = kadane(arr, n);

    int total_sum = 0;
    int min_ending_here = *arr;
    int min_so_far = *arr;

    for (int i = 0; i < n; i++) {
        total_sum += *arr;
        min_ending_here = min(*arr, min_ending_here + *arr);
        min_so_far = min(min_so_far, min_ending_here);
        arr++;
    }

    if (total_sum == min_so_far) {
        return max_kadane;
    }

    int max_circular = total_sum - min_so_far;
    return max(max_kadane, max_circular);
}

int main() {
    int n;
    cout << "Enter the size of the circular array: ";
    cin >> n;

    int* arr = new int[n];
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int result = maxCircularSubarraySum(arr, n);
    cout << "The maximum sum of a non-empty subarray in the circular array is: " << result << endl;

    // Memory cleanup
    delete[] arr;

    return 0;
}