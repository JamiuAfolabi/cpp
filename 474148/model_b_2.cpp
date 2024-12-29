#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

void fisherYatesShuffle(int* arr, size_t n) {
    for (size_t i = n - 1; i > 0; --i) {
        // Generate a random index between 0 and i
        size_t j = rand() % (i + 1);

        // Swap arr[i] with the randomly selected element
        std::swap(arr[i], arr[j]);
    }
}

int main() {
    const size_t arraySize = 10; // Adjust the size as needed

    std::cout << "Original array: ";
    // Dynamically allocate memory for the array
    int* numbers = new int[arraySize];

    // Initialize the array with consecutive numbers
    for (size_t i = 0; i < arraySize; ++i) {
        numbers[i] = static_cast<int>(i + 1);
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(0)));

    fisherYatesShuffle(numbers, arraySize);

    std::cout << "Shuffled array: ";
    // Print the shuffled array
    for (size_t i = 0; i < arraySize; ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Don't forget to free the allocated memory
    delete[] numbers;
    return 0;
}
