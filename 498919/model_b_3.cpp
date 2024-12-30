#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

void replaceValue(int *arr, int start, int end, int oldValue, int newValue, mutex& mtx) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == oldValue) {
            arr[i] = newValue;
        }
    }
}

void parallelReplaceValue(int* arr, int size, int oldValue, int newValue) {
    const int threadCount = thread::hardware_concurrency();
    const int chunkSize = size / threadCount + 1;

    mutex mtx;
    vector<thread> threads;

    for (int threadId = 0; threadId < threadCount; threadId++) {
        int start = threadId * chunkSize;
        int end = min((threadId + 1) * chunkSize - 1, size - 1);
        threads.push_back(thread([&, start, end, oldValue, newValue, &mtx] {
            replaceValue(arr, start, end, oldValue, newValue, mtx);
        }));
    }

    for (thread& t : threads) {
        t.join();
    }
}

void printArray(const int* arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    const int SIZE = 10;  // Using a large array for testing
    int arr[SIZE] = { 1,2,3,4,5,1,2,3,1,2};

    cout << "Original Array: ";
    printArray(arr, SIZE);

    int oldValue, newValue;
    cout << "Enter the value to replace: ";
    cin >> oldValue;
    cout << "Enter the new value: ";
    cin >> newValue;

    // Start timer to measure execution time
    // ...

    parallelReplaceValue(arr, SIZE, oldValue, newValue);

    // Stop timer and print execution time
    // ...

    cout << "Modified Array: ";
    printArray(arr, SIZE);

    return 0;
}