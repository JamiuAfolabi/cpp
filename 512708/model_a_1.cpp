#include <iostream>
#include <vector>

int main() {
    int n;
    std::cout << "Enter the number of elements in the array: ";
    std::cin >> n;

    std::vector<int> arr(n);
    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    // Compute the initial value for curr_val and sum_of_array
    int curr_val = 0;
    int sum_of_array = 0;
    
    for (int i = 0; i < n; ++i) {
        curr_val += i * arr[i];
        sum_of_array += arr[i];
    }

    int max_val = curr_val;

    // Calculate value for all rotations
    for (int i = 1; i < n; ++i) {
        curr_val = curr_val + sum_of_array - n * arr[n - i]; // Rotate
        max_val = std::max(max_val, curr_val);
    }

    std::cout << "Maximum value of i * arr[i] for all rotations: " << max_val << std::endl;

    return 0;
}