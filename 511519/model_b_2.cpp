#include <iostream>
#include <vector>

void cloneAndModifyArray(const std::vector<int>& original) {
    // Cloning the array using std::vector constructor
    std::vector<int> clonedArray(original);

    // Now you can modify the cloned array
    // Example: Replace each element with its double
    for (size_t i = 0; i < clonedArray.size(); ++i) {
        clonedArray[i] *= 2;
    }

    // Display the cloned and modified array
    std::cout << "Modified Cloned Array: ";
    for (const int& value : clonedArray) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Original array
    std::vector<int> originalArray = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    // Call the function to clone and modify the array as many times as you need
    cloneAndModifyArray(originalArray);

    // Display the original array to show it hasn't changed
    std::cout << "Original Array: ";
    for (const int& value : originalArray) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
