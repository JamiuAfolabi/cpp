#include <iostream>
#include <vector>

// Function to perform jump search
int jumpSearch(const std::vector<int>& arr, int target) {
    int size = arr.size();

    // If the array is empty or contains a single element, return index 0 if found, else -1
    if (size == 0 || size == 1) {
        return arr[0] == target ? 0 : -1;
    }

    int block_size = static_cast<int>(std::sqrt(size));
    int* start = arr.data();
    int* end = arr.data() + size - 1;

    while (start < end) {
        // Calculate the end of the current block
        int* block_end = std::min(start + block_size, end);

        // If the target is in the current block, perform binary search to find it
        if (arr[*block_end] >= target) {
            int low = 0;
            int high = block_size - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                if (arr[start + mid] == target) {
                    return start + mid - arr.data();
                } else if (arr[start + mid] < target) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
        }

        // If the target is not found in the current block, move to the next block
        start += block_size;
    }

    // If the target is not found in any block, return -1
    return -1;
}

int main() {
    // Example sorted array
    std::vector<int> orders = {0, 1, 3, 5, 8, 13, 21, 34, 55, 89};

    int target;
    std::cout << "Enter the order number to search: ";
    std::cin >> target;

    // Perform jump search
    int result = jumpSearch(orders, target);

    if (result != -1) {
        std::cout << "Order number " << target << " found at index: " << result << std::endl;
    } else {
        std::cout << "Order number " << target << " not found." << std::endl;
    }

    return 0;
}