#include <iostream>

void cloneAndModifyArray(const int* original, int size) {
    // Create a new array to clone the original
    int* clonedArray = new int[size];

    // Copy elements from the original array to the cloned array
    for (int i = 0; i < size; ++i) {
        clonedArray[i] = original[i];
    }

    // Now you can modify the cloned array as needed
    // Example: Replace each element with its double
    for (int i = 0; i < size; ++i) {
        clonedArray[i] *= 2;
    }

    // Display the cloned and modified array
    std::cout << "Modified Cloned Array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << clonedArray[i] << " ";
    }
    std::cout << std::endl;

    // Clean up dynamically allocated memory
    delete[] clonedArray;
}

int main() {
    // Original array
    int originalArray[] = {1, 2, 3, 4, 5};
    int size = sizeof(originalArray) / sizeof(originalArray[0]);

    // Call the function to clone and modify the array
    cloneAndModifyArray(originalArray, size);

    // Display the original array to show it hasn't changed
    std::cout << "Original Array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << originalArray[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}