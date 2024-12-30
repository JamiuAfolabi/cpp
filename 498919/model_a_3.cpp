#include <iostream>
#include <thread>
#include <vector>

void replaceValueSegment(int *arr, int start, int end, int oldValue, int newValue) {
    for (int i = start; i < end; ++i) {
        if (arr[i] == oldValue) {
            arr[i] = newValue;
        }
    }
}

void parallelReplaceValue(int *arr, int size, int oldValue, int newValue) {
    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    int segmentSize = size / numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * segmentSize;
        int end = (i == numThreads - 1) ? size : start + segmentSize; // Handle last segment
        threads.emplace_back(replaceValueSegment, arr, start, end, oldValue, newValue);
    }
    
    for (auto& t : threads) {
        t.join(); // Wait for all threads to finish
    }
}

void printArray(const int *arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int SIZE = 20;
    int arr[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                     1, 2, 1, 4, 1, 6, 1, 8, 1, 10}; // Sample array
    
    std::cout << "Original Array: ";
    printArray(arr, SIZE);
    
    int oldValue, newValue;

    std::cout << "Enter the value to replace: ";
    std::cin >> oldValue;
    std::cout << "Enter the new value: ";
    std::cin >> newValue;

    parallelReplaceValue(arr, SIZE, oldValue, newValue);

    std::cout << "Modified Array: ";
    printArray(arr, SIZE);

    return 0;
}