#include <iostream>
using namespace std;

void replaceValue(int* arr, int size, int oldValue, int newValue) {
    // using the pointer to start of the array
    int* end = arr + size;

    for (int* it = arr; it != end; ++it) {
        if (*it == oldValue) {
            *it = newValue;
        }
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    const int SIZE = 20;
    int arr[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                     1, 2, 1, 4, 1, 6, 1, 8, 1, 10}; // Sample array
    
    cout << "Original Array: ";
    printArray(arr, SIZE);
    
    int oldValue, newValue;

    cout << "Enter the value to replace: ";
    cin >> oldValue;
    cout << "Enter the new value: ";
    cin >> newValue;

    replaceValue(arr, SIZE, oldValue, newValue);

    cout << "Modified Array: ";
    printArray(arr, SIZE);

    return 0;
}