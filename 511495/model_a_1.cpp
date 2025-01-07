#include <iostream>
#include <vector>

// Function to perform binary search
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; // To avoid overflow

        // Check if target is present at mid
        if (arr[mid] == target) {
            return mid; // Target found, return index
        }
        // If target is greater, ignore left half
        else if (arr[mid] < target) {
            left = mid + 1;
        }
        // If target is smaller, ignore right half
        else {
            right = mid - 1;
        }
    }

    return -1; // Target not found
}

int main() {
    // Example sorted array
    std::vector<int> orders = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

    int target;
    std::cout << "Enter the order number to search: ";
    std::cin >> target;

    // Perform binary search
    int result = binarySearch(orders, target);

    if (result != -1) {
        std::cout << "Order number " << target << " found at index: " << result << std::endl;
    } else {
        std::cout << "Order number " << target << " not found." << std::endl;
    }

    return 0;
}