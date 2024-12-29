#include <iostream>
#include <vector>

using namespace std;

#define INSERTION_SORT_THRESHOLD 30  // Threshold to switch between Insertion Sort and QuickSort

void insertionSort(vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        arr[i] = arr[j];
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void sortArrayWithThreshold(vector<int>& arr) {
    int size = arr.size();
    if (size > INSERTION_SORT_THRESHOLD) {
        quickSort(arr, 0, size - 1);
    } else {
        insertionSort(arr, 0, size - 1);
    }
}


int main() {
    vector<int> arr = {8, 3, 2, 5, 4, 7, 6, 1};

    cout << "Before Sorting: \n";
    for (int i : arr) {
        cout << i << " ";
    }

    sortArrayWithThreshold(arr);

    cout << "\nAfter Sorting: \n";
    for (int i : arr) {
        cout << i << " ";
    }
    return 0;
}
  