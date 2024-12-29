#include <iostream>
#include <random>

void fisherYatesShuffle(int* arr, int size) {
    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 rng(rd());  // Seed the generator

    for (int i = size - 1; i > 0; --i) {
        // Generate a random index from 0 to i (inclusive)
        std::uniform_int_distribution<int> uni(0, i);
        int j = uni(rng);

        // Swap the elements at arr[i] and arr[j] using pointer arithmetic
        std::swap(*(arr + i), *(arr + j));
    }
}

int main() {
    int size = 10; // Size of the array
    int* numbers = new int[size]; // Dynamic memory allocation

    // Initialize the array
    for (int i = 0; i < size; ++i) {
        *(numbers + i) = i + 1; // Filling with values 1 to 10
    }

    std::cout << "Original array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << *(numbers + i) << " "; // Accessing array elements using pointer
    }
    std::cout << std::endl;

    fisherYatesShuffle(numbers, size);

    std::cout << "Shuffled array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << *(numbers + i) << " "; // Accessing array elements using pointer
    }
    std::cout << std::endl;

    delete[] numbers; // Free the allocated memory

    return 0;
}