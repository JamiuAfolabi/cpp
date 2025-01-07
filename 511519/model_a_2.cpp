#include <iostream>
#include <vector>
#include <memory>

void cloneAndModifyVector(const std::vector<int>& original) {
    // Clone the original vector using the copy constructor
    std::vector<int> clonedVector = original;

    // Modify the cloned vector
    for (auto& value : clonedVector) {
        value *= 2; // Example: double each element
    }

    // Display the modified cloned vector
    std::cout << "Modified Cloned Vector: ";
    for (const auto& value : clonedVector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Original vector
    std::vector<int> originalVector = {1, 2, 3, 4, 5};

    // Call the function to clone and modify the vector
    cloneAndModifyVector(originalVector);

    // Display the original vector to show it hasn't changed
    std::cout << "Original Vector: ";
    for (const auto& value : originalVector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}