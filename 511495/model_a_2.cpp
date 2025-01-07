#include <iostream>
#include <cmath> // For sqrt function

// Function to perform Jump Search
int jumpSearch(int* arr, int size, int target) {
    // Finding the step size to jump
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
        // If we've reached the end of the block or array, return -1
        if (prev == std::min(step, size)) {
            return -1;
        }
    }

    // If found, return the index
    if (arr[prev] == target) {
        return prev;
    }

    return -1; // If not found
}

int main() {
    // Example sorted array
    int orders[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int size = sizeof(orders) / sizeof(orders[0]);

    int target;
    std::cout << "Enter the order number to search: ";
    std::cin >> target;

    // Perform Jump Search
    int result = jumpSearch(orders, size, target);

    if (result != -1) {
        std::cout << "Order number " << target << " found at index: " << result << std::endl;
    } else {
        std::cout << "Order number " << target << " not found." << std::endl;
    }

    return 0;
}