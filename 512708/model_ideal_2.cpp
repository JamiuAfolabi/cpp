#include <iostream>
using namespace std;

// Function to calculate the maximum sum based on the array and pivot
int maxSum(int* arr, int n);

// Function to find the pivot point in the array
int findPivot(int* arr, int n);

int maxSum(int* arr, int n) 
{ 
    int sum = 0; 
    int* ptr = arr;
    int pivot = findPivot(arr, n); // Find the pivot index
    int diff = n - 1 - pivot; // Calculate the difference based on pivot
    for(int i = 0; i < n; i++) 
    { 
        sum = sum + ((i + diff) % n) * *(ptr + i); // Sum calculation using the pivot
    } 
    return sum; // Return the maximum sum
} 

int findPivot(int* arr, int n) 
{ 
    int* ptr = arr;
    for(int i = 0; i < n; i++) 
    { 
        // Check if the current element is greater than the next one (modular comparison)
        if(*(ptr + i) > *(ptr + (i + 1) % n)) 
            return i; // Return the pivot index
    }
    return -1; // Return -1 if no pivot is found
}

// Main function to input the array and print the maximum sum
int main(void) 
{ 
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    
    int arr[n];
    cout << "Enter the elements of the array: ";
    for(int i = 0; i < n; i++) {
        cin >> arr[i]; // Input array elements
    }

    int max = maxSum(arr, n); // Calculate the maximum sum
    cout << "The maximum sum is: " << max << endl; // Print the result
    return 0; 
}
