#include <iostream>

void findMaximumForRotatedArray(int* arr, int n) {
    // Edge case for empty array or single element
    if (n == 0) return;
    if (n == 1) {
        std::cout << "Maximum value of i * arr[i]: " << 0 << std::endl;
        return;
    }

    // Calculate initial sums
    int curr_val = 0;
    int sum_of_array = 0;

    for (int i = 0; i < n; ++i) {
        curr_val += i * arr[i];
        sum_of_array += arr[i];
    }

    int max_val = curr_val; // Initialize max_val

    // Calculate values for all rotations
    for (int i = 1; i < n; ++i) {
        curr_val = curr_val + sum_of_array - n * arr[n - i];
        if (curr_val > max_val) max_val = curr_val;
    }

    std::cout << "Maximum value of i * arr[i] for all rotations: " << max_val << std::endl;
}

int findPivot(int* arr, int n) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if mid is pivot
        if (mid < right && arr[mid] > arr[mid + 1])
            return mid;
        if (mid > left && arr[mid] < arr[mid - 1])
            return mid - 1;

        // Decide to go left or right
        if (arr[left] >= arr[mid])
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1; // Array not rotated
}

void findMaximumUsingPivot(int* arr, int n) {
    int pivotIndex = findPivot(arr, n);
    
    // If the array is not rotated
    if (pivotIndex == -1) {
        findMaximumForRotatedArray(arr, n);
        return;
    }

    // You can then calculate maximum using curr_val
    // using pivotIndex to maximize across left and right.

    // Your original method can be applied here
}

int main() {
    int n;
    std::cout << "Enter the number of elements in the array: ";
    std::cin >> n;

    // Dynamically allocate an array
    int* arr = new int[n];

    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    findMaximumUsingPivot(arr, n);

    // Clean up allocated memory
    delete[] arr;

    return 0;
}