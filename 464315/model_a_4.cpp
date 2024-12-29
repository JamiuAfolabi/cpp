#include <iostream>
#include <vector>
#include <thread>
#include <algorithm> // For std::swap

// Define constants
#define INSERTION_THRESHOLD 10
#define THREAD_THRESHOLD 1000

// Function to perform Insertion Sort
void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Function to perform partition for QuickSort
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Choosing the last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Function to perform QuickSort
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        if (high - low + 1 <= INSERTION_THRESHOLD) {
            insertionSort(arr, low, high); // Use Insertion Sort for small datasets
        } else {
            // Partitioning the array and getting the pivot index
            int pi = partition(arr, low, high);

            std::thread t1, t2;

            // Create threads only if the size is larger than THREAD_THRESHOLD
            if (pi - 1 > low && high - low + 1 > THREAD_THRESHOLD) {
                t1 = std::thread(quickSort, std::ref(arr), low, pi - 1);
            } else {
                quickSort(arr, low, pi - 1); // Process in the same thread
            }

            if (pi + 1 < high && high - low + 1 > THREAD_THRESHOLD) {
                t2 = std::thread(quickSort, std::ref(arr), pi + 1, high);
            } else {
                quickSort(arr, pi + 1, high); // Process in the same thread
            }

            // Wait for threads to finish
            if (t1.joinable()) t1.join();
            if (t2.joinable()) t2.join();
        }
    }
}

// Function to perform Hybrid Sort
void hybridSort(std::vector<int>& arr, int left, int right) {
    quickSort(arr, left, right);
}

int main() {
    std::vector<int> stockPrices = { 3, 6, 8, 10, 1, 2, 1, 5, 9, 7, 4, 12, 11, 15, 14 };

    std::cout << "Unsorted stock prices: ";
    for (int price : stockPrices) {
        std::cout << price << " ";
    }
    std::cout << std::endl;

    // Apply the hybrid sort
    hybridSort(stockPrices, 0, stockPrices.size() - 1);

    std::cout << "Sorted stock prices: ";
    for (int price : stockPrices) {
        std::cout << price << " ";
    }
    std::cout << std::endl;

    return 0;
}