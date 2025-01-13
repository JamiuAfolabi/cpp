#include <iostream>
using namespace std;

// Function to calculate the maximum sum based on the array and pivot
int maxSum(int* arr, int n);

// Function to find the pivot point in the array using binary search
int findPivot(int* arr, int n);

// Function to check if the array is sorted
bool isSorted(int* arr, int n);

// Function to check if the array is a rotated sorted array
bool isRotatedSorted(int* arr, int n);

// Function to check if the input is valid
bool isValidInput(int n);

// Function to deallocate the dynamically allocated array
void deallocateArray(int* arr);

int maxSum(int* arr, int n) 
{ 
    int sum = 0; 
    int* ptr = arr;
    
    // Check if the array is sorted or rotated sorted
    if (!isSorted(arr, n) && !isRotatedSorted(arr, n)) {
        // Brute force approach if the array is neither sorted nor rotated sorted
        cout << "Array is neither sorted nor rotated sorted, using brute force..." << endl;

        // Compute the initial value for curr_val and sum_of_array
        int curr_val = 0;
        int sum_of_array = 0;
        
        for (int i = 0; i < n; ++i) {
            curr_val += i * arr[i];
            sum_of_array += arr[i];
        }
        cout << "Initial curr_val: " << curr_val << endl;
        cout << "Initial sum_of_array: " << sum_of_array << endl;

        int max_val = curr_val;

        // Calculate value for all rotations
        for (int i = 1; i < n; ++i) {
            curr_val = curr_val + sum_of_array - n * arr[n - i]; // Rotate
            cout << "Rotation " << i << ": curr_val = " << curr_val << endl;
            max_val = max(max_val, curr_val);
        }

        return max_val;
    } else {
        // Use binary search to find pivot if sorted or rotated sorted
        cout << "Array is sorted or rotated sorted, using binary search to find pivot..." << endl;
        int pivot = findPivot(arr, n); 
        cout << "Pivot found at index: " << pivot << endl;

        int diff = n - 1 - pivot; 
        for(int i = 0; i < n; i++) 
        { 
            sum = sum + ((i + diff) % n) * *(ptr + i); 
        }
    }
    return sum; 
} 

int findPivot(int* arr, int n) 
{ 
    int low = 0, high = n - 1;
    
    // Binary search for finding the pivot point
    while (low <= high) 
    { 
        int mid = low + (high - low) / 2;
        cout << "Binary Search - low: " << low << ", mid: " << mid << ", high: " << high << endl;
        
        // Check if mid is the pivot
        if (arr[mid] > arr[(mid + 1) % n]) 
            return mid; 
        if (arr[mid] < arr[low]) 
            high = mid - 1;
        else 
            low = mid + 1;
    }
    return -1; // Return -1 if no pivot is found (array is sorted)
}

bool isSorted(int* arr, int n) 
{
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) 
            return false;
    }
    return true;
}

bool isRotatedSorted(int* arr, int n)
{
    int count = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            count++;
        }
    }
    // The array is a rotated sorted array if it has exactly one "drop"
    return (count == 1) || (count == 0); 
}

bool isValidInput(int n)
{
    if (n <= 0) {
        cout << "Invalid input. Array size should be positive." << endl;
        return false;
    }
    return true;
}

void deallocateArray(int* arr) {
    if (arr != nullptr) {
        delete[] arr;
        cout << "Memory deallocated successfully." << endl;
    }
}

// Main function to input the array and print the maximum sum
int main(void) 
{ 
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    // Check if the input is valid
    if (!isValidInput(n)) return -1;

    // Dynamic memory allocation
    int* arr = new int[n];

    cout << "Enter the elements of the array: ";
    for(int i = 0; i < n; i++) {
        cin >> arr[i]; // Input array elements
    }

    int max = maxSum(arr, n); // Calculate the maximum sum
    cout << "The maximum sum is: " << max << endl; // Print the result

    deallocateArray(arr);

    return 0; 
}