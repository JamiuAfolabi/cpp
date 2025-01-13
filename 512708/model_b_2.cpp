#include <iostream>

int main() {
    int n;
    std::cout << "Enter the number of elements in the array: ";
    std::cin >> n;

    // Create a raw array using new
    int* arr = new int[n];
    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    // Find the pivot using binary search (pivot is the smallest element)
    int pivot = findPivot(arr, 0, n - 1);

    // Calculate the maximum value of i * arr[i] for all rotations
    int max_val = findMaxSumWithPivot(arr, n, pivot);

    std::cout << "Maximum value of i * arr[i] for all rotations: " << max_val << std::endl;

    // Delete the raw array to prevent memory leak
    delete[] arr;

    return 0;
}

// Function to find the pivot index using binary search
int findPivot(int* arr, int low, int high) {
    if (low == high) {
        return low;
    }

    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[high]) {
        return findPivot(arr, mid + 1, high);
    } else if (arr[mid] > arr[high]) {
        return findPivot(arr, low, mid - 1);
    } else {
        // Duplicate pivot, so we return the leftmost one
        return findPivot(arr, low, mid);
    }
}

int findMaxSumWithPivot(int* arr, int n, int pivot) {
    int curr_sum = 0;
    int prev_sum = 0;

    // Calculate sum from pivot to the end
    for (int i = pivot; i < n; ++i) {
        curr_sum += arr[i];
    }

    // Calculate sum from the start to pivot-1
    for (int i = pivot - 1; i >= 0; --i) {
        curr_sum += arr[i];
    }

    // Calculate the first term of the sliding window
    int first_term = (n - pivot) * arr[pivot];

    max_val = first_term + curr_sum;

    // Iterate for all possible rotations and update max_val
    for (int i = pivot - 1; i >= 0; --i) {
        prev_sum += arr[i];
        curr_sum -= arr[n - 1];
        max_val = std::max(max_val, first_term + curr_sum - prev_sum);
    }

    return max_val;
}
