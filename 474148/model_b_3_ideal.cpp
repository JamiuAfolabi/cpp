#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>

void fisherYatesShuffleOptimized(int* arr, size_t n) {
    for (size_t i = n - 1; i > 0; --i) {
        size_t j = rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
}

void fisherYatesShuffleUnoptimized(std::vector<int>& arr) {
    std::random_device rd;
    std::mt19937 rng(rd());
    for (int i = arr.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<int> uni(0, i);
        int j = uni(rng);
        std::swap(arr[i], arr[j]);
    }
}

// Overloaded benchmarkShuffle function for int* and size_t (optimized shuffle)
double benchmarkShuffle(void (*shuffleFunc)(int*, size_t), int* arr, size_t n) {
    auto start = std::chrono::high_resolution_clock::now();
    shuffleFunc(arr, n);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count(); // Return time in microseconds
}

// Overloaded benchmarkShuffle function for std::vector<int>& (unoptimized shuffle)
double benchmarkShuffle(void (*shuffleFunc)(std::vector<int>&), std::vector<int>& arr) {
    auto start = std::chrono::high_resolution_clock::now();
    shuffleFunc(arr);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count(); // Return time in microseconds
}

int main() {
    const size_t arraySize = 10000;

    // Benchmark unoptimized version
    std::vector<int> unoptimizedArray(arraySize);
    for (size_t i = 0; i < arraySize; ++i) {
        unoptimizedArray[i] = static_cast<int>(i + 1);
    }
    double unoptimizedTime = benchmarkShuffle(fisherYatesShuffleUnoptimized, unoptimizedArray);

    // Benchmark optimized version
    int* optimizedArray = new int[arraySize];
    for (size_t i = 0; i < arraySize; ++i) {
        optimizedArray[i] = static_cast<int>(i + 1);
    }
    double optimizedTime = benchmarkShuffle(fisherYatesShuffleOptimized, optimizedArray, arraySize);

    // Display results
    std::cout << "Unoptimized version time: " << unoptimizedTime << " microseconds." << std::endl;
    std::cout << "Optimized version time: " << optimizedTime << " microseconds." << std::endl;

    if (unoptimizedTime < optimizedTime) {
        std::cout << "Unoptimized version is faster." << std::endl;
    } else {
        std::cout << "Optimized version is faster." << std::endl;
    }

    // Clean up dynamically allocated memory
    delete[] optimizedArray;

    return 0;
}
