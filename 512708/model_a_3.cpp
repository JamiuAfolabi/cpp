#include <iostream>
using namespace std;

// Function declarations
int maxSum(int* arr, int n);
int findPivot(int* arr, int n);
bool isSortedOrRotated(int* arr, int n);

int maxSum(int* arr, int n) 
{ 
    if (n == 0) return 0;  // Handle empty array case

    // Check if the array is sorted or rotated
    if (!isSortedOrRotated(arr, n)) {
        cout << "Array is not sorted or rotated sorted." << endl;
        return 0; // Or handle as needed (e.g., throw an error)
    }

    int pivot = findPivot(arr, n); // Find the pivot index
    if (pivot == -1) {
        cout << "No pivot found, treating as sorted." << endl;
        pivot = n - 1; // Treat as fully sorted (no rotation)
    }

    // Calculate the maximum sum
    int sum = 0;
    for (int i = 0; i < n; i++) { 
        sum += ((i + (n - 1 - pivot)) % n) * arr[i];
    }
    return sum; // Return the maximum sum
} 

int findPivot(int* arr, int n) 
{ 
    int left = 0, right = n - 1;
    
    // Binary search algorithm to find the pivot
    while (left <= right) {
        int mid = left + (right - left) / 2;

        // If mid is greater than next element, then mid is pivot
        if (mid < right && arr[mid] > arr[mid + 1]) {
            return mid;
        }
        // If mid is less than previous element, then mid-1 is pivot
        if (mid > left && arr[mid] < arr[mid - 1]) {
            return mid - 1;
        }
        
        // Decide which half to search
        if (arr[left] >= arr[mid]) {
            right = mid - 1; // Move left
        } else {
            left = mid + 1; // Move right
        }
    }
    return -1; // No pivot found
}

bool isSortedOrRotated(int* arr, int n) 
{
    if (n <= 1) return true; // An array of size 0 or 1 is sorted

    int count = 0; // Count the number of times the order is violated
    for (int i = 0; i < n; i++) {
        if (arr[i] > arr[(i + 1) % n]) {
            count++;
        }
    }
    return count <= 1; // At most one violation for rotated sorted array
}

// Main function
int main(void) 
{ 
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    if (n <= 0) {
        cout << "Array size must be greater than zero." << endl;
        return 1; // Exit if the array size is invalid
    }

    int* arr = new int[n]; // Use dynamic allocation for the array
    cout << "Enter the elements of the array: ";
    for(int i = 0; i < n; i++) {
        cin >> arr[i]; // Input array elements
    }

    int max = maxSum(arr, n); // Calculate the maximum sum
    cout << "The maximum sum is: " << max << endl; // Print the result

    delete[] arr; // Clean up allocated memory
    return 0; 
}