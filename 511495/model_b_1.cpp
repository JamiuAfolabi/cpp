
#include <iostream>
using namespace std;

int binary_search(int sorted_array[], int low, int high, int key)
{
    if (low > high)
        return -1;

    int mid = (low + high) / 2;
    if (sorted_array[mid] == key)
        return mid;
    else if (sorted_array[mid] < key)
        return binary_search(sorted_array, mid + 1, high, key);
    else
        return binary_search(sorted_array, low, mid - 1, key);
}

int main()
{
    int n;
    cout << "Enter the number of orders: ";
    cin >> n;

    int* orders = new int[n];

    cout << "Enter all the order numbers separated by space: ";
    for (int i = 0; i < n; i++) {
        cin >> orders[i];
    }
    cout << "Orders array sorted in ascending order: ";
    for (int i = 0; i < n; i++) {
        cout << orders[i] << " ";
    }

    int key;
    cout << "\nEnter the order number to search: ";
    cin >> key;

    int index = binary_search(orders, 0, n - 1, key);

    if (index == -1)
        cout << "\nOrder number " << key << " not found." << endl;
    else
        cout << "\nOrder number " << key << " found at index " << index << endl;

    delete[] orders;

    return 0;
}
