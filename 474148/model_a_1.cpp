#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

void fisherYatesShuffle(std::vector<int>& arr) {
    // Get the random number generator
    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 rng(rd());  // Seed the generator

    for (int i = arr.size() - 1; i > 0; --i) {
        // Generate a random index from 0 to i (inclusive)
        std::uniform_int_distribution<int> uni(0, i);
        int j = uni(rng);

        // Swap arr[i] with the element at the random index
        std::swap(arr[i], arr[j]);
    }
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Original array: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    fisherYatesShuffle(numbers);

    std::cout << "Shuffled array: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}