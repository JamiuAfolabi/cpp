#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

void fisherYatesShuffle_optimized(int* arr, size_t n) {
    for (size_t i = n - 1; i > 0; --i) {
        size_t j = rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
}

void fisherYatesShuffle_unoptimized(std::vector<int>& arr) {
    std::random_device rd;  
    std::mt19937 rng(rd());
    
    for (int i = arr.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<int> uni(0, i);
        int j = uni(rng);
        std::swap(arr[i], arr[j]);
    }
}

double compareExecutionTime(void (*shuffleFunction)(std::vector<int>&), std::vector<int>& array) {
    auto start = std::chrono::high_resolution_clock::now();
    shuffleFunction(array);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(duration.count()) / 1000.0; // Return in milliseconds
}

bool isBetter(double time1, double time2) {
    return time1 < time2;
}

int main() {
    const size_t arraySize = 10000;

    // Unoptimized version
    std::vector<int> unoptimizedArray(arraySize);
    for (size_t i = 0; i < arraySize; ++i) {
        unoptimizedArray[i] = i + 1;
    }

    double unoptimizedTime = compareExecutionTime(fisherYatesShuffle_unoptimized, unoptimizedArray);

    // Optimized version
    int* optimizedArray = new int[arraySize];
    for (size_t i = 0; i < arraySize; ++i) {
        optimizedArray[i] = i + 1;
    }

    std::srand(static_cast<unsigned>(std::time(0)));
    double optimizedTime = compareExecutionTime(fisherYatesShuffle_optimized, optimizedArray);

    delete[] optimizedArray;

    std::cout << "Unoptimized version execution time: " << unoptimizedTime << " ms" << std::endl;
    std::cout << "Optimized version execution time: " << optimizedTime << " ms" << std::endl;
    
    std::cout << (isBetter(optimizedTime, unoptimizedTime) ? "Optimized" : "Unoptimized") << " version is better" << std::endl;

    return 0;
}