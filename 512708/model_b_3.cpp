#include <iostream>

using namespace std;

bool arrayIsSorted(int arr[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    if (arr[i] > arr[i + 1])
    {
      return false;
    }
  }
  return true;
}
bool isRotatedSortedArray(int arr[], int n)
{
  if (n <= 1)
    return true;

  int low = 0;
  int high = n - 1;
  while (low < high)
  {
    int mid = (low + high) / 2;
    if (arr[mid] < arr[high])
    {
      high = mid;
    }
    else if (arr[mid] > arr[high])
    {
      low = mid + 1;
    }
    else
    {
      high--;
    }
  }
  // This while loop will check if the element next to the pivot is greater,
  // resulting in something resembling "3, 3, 3, 0, 1, 2". We exclude this case.
  while (low > 0 && arr[low - 1] == arr[low]) {
    low--;
  }

  // After taking one pass and removing duplicate retation points, we check for sroted-ness to left and right sides
  return arrayIsSorted(arr, low) && arrayIsSorted(arr + low, n - low);
}
int findPivot_binary_search(int arr[], int n)
{
  int low = 0;
  int high = n - 1;
  while (low <= high)
  {
    int mid = low + (high - low) / 2;

    if ((mid == 0 || arr[mid - 1] > arr[mid]) && (mid == n - 1 || arr[mid] < arr[mid + 1]))
    {
      return mid;
    }
    else if (arr[low] <= arr[mid])
    {
      low = mid + 1;
    }
    else
    {
      high = mid - 1;
    }
  }

  // Array is sorted or all elements are equal
  return -1;
}
int maxSum(int* arr, int n)
{
  if (n == 0)
  {
    return 0;
  }
  int pivot = findPivot_binary_search(arr, n);
  if (pivot == -1) {
    std::cout << "Warning: The array is not sorted or contains duplicate elements, can't find pivot" << std::endl;
    return 0; // Pivot not found, return some appropriate value to indicate an error.
  }
  cout << "Pivot element is " << arr[pivot] << endl;

  int sum = 0;
  int diff = n - 1 - pivot;
  for (int i = 0; i < n; i++)
  {
    sum += ((i + diff) % n) * arr[i];
  }
  return sum;
}
int main()
{
  int n;
  cout << "Enter the size of the array: ";
  cin >> n;

  if (n <= 0)
  {
    cout << "Invalid input: Array size should be positive. Please try again." << endl;
    return 1;
  }

  int arr[n];
  cout << "Enter the elements of the array: ";
  for (int i = 0; i < n; i++)
  {
    cin >> arr[i];
  }

  if (isRotatedSortedArray(arr, n))
  {
    int max_sum = maxSum(arr, n);
    cout << "The maximum sum is: " << max_sum << endl;
  }
  else
  {
    cout << "Warning: The array is not a rotated sorted array." << endl;
  }

  return 0;
}