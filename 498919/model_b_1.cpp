#include<iostream>
using namespace std;

void bulkReplace(int arr[], int n, int old_val, int new_val)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == old_val)
        {
            arr[i] = new_val;
        }
    }
}
 
int main()
{
    const int size = 20;
    int arr[size];
    int old_value, new_value;

    // Display the original array
    cout << "Original Array: \n";
    for (int i = 0; i < size; i++)
    {
        cin >> arr[i];
        cout << arr[i] << " ";
    }
    cout << endl;

    // Input the element to be replaced and its new value
    cout << "Enter the element to be replaced: ";
    cin >> old_value;
    cout << "Enter the new value: ";
    cin >> new_value;

    // Function to perform the bulk replacement
    bulkReplace(arr, size, old_value, new_value);

    // Display the array after replacement
    cout << "Array after replacement: \n";
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}