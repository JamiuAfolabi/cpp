#include <iostream>
#include <vector>
#include <future>

// Define a constant for the threshold size
#define THRESHOLD 10

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

// Function to perform QuickSort
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

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void hybridSort(std::vector<int>& arr, int left, int right) {
    if (right - left + 1 <= THRESHOLD) {
        insertionSort(arr, left, right);
    } else {
        int pi = partition(arr, left, right);

        auto leftTask = std::async(std::launch::async, quickSort, std::ref(arr), left, pi - 1);
        auto rightTask = std::async(std::launch::async, quickSort, std::ref(arr), pi + 1, right);

        leftTask.get();
        rightTask.get();
    }
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