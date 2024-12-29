#include <iostream>
#include <vector>
#include <thread>
#include <algorithm> // For std::swap
#include <mutex> // For mutex

// Define constants for threshold sizes
#define THRESHOLD_SEQUENTIAL 10 // Threshold for sequential sorting
#define THRESHOLD_PARALLEL 1000 // Threshold for parallel sorting
#define MIN_PARTITION_SIZE 1000 // Minimum partition size to spawn threads

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
    int pivot = arr[high];
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

// Function to perform QuickSort recursively
void quickSortSequential(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortSequential(arr, low, pi - 1);
        quickSortSequential(arr, pi + 1, high);
    }
}

void quickSortParallel(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Create threads only for partitions larger than MIN_PARTITION_SIZE
        int left_size = pi - low;
        int right_size = high - pi;

        if (left_size > MIN_PARTITION_SIZE) {
            std::thread t1(quickSortParallel, std::ref(arr), low, pi - 1);
            t1.detach();
        } else {
            quickSortSequential(arr, low, pi - 1);
        }

        if (right_size > MIN_PARTITION_SIZE) {
            std::thread t2(quickSortParallel, std::ref(arr), pi + 1, high);
            t2.detach();
        } else {
            quickSortSequential(arr, pi + 1, high);
        }
    }
}

// Function to perform Hybrid Sort
void hybridSort(std::vector<int>& arr, int left, int right) {
    if (right - left + 1 <= THRESHOLD_SEQUENTIAL) {
        insertionSort(arr, left, right);
    } else if (right - left + 1 <= THRESHOLD_PARALLEL) {
        quickSortSequential(arr, left, right);
    } else {
        quickSortParallel(arr, left, right);
    }
} 
 int main() {
        std::vector<int> stockPrices = { 3, 6, 8, 10, 1, 2, 1, 5, 9, 7, 4, 12, 11, 15, 14 ,
                                         /* Add more elements for a larger dataset */
        };
  // Rest of the code remains the same.
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