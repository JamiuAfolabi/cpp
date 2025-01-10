#include<bits/stdc++.h>
using namespace std;
int maxSubArraySum(vector<int> &arr)
{
    int current_sum = arr[0];
    int max_sum = arr[0];
    for (int i = 1; i < arr.size(); i++)
    {
        current_sum = max(arr[i], current_sum + arr[i]);
        max_sum = max(max_sum, current_sum);
    }

    // If the array contains all negative numbers,
    // the sum of the entire array will be negative.
    // In such a case, the maximum sum would be the
    // maximum element itself.
    return max(max_sum, *max_element(arr.begin(), arr.end()));
}
int main()
{
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter all the elements separated by space: ";
    for (int &x : arr)
        cin >> x;

    //If array is empty return 0
    if(n == 0)
    {
        cout << "Maximum sum is 0" << endl;
        return 0;
    }
 
    cout << "Maximum sum of a non-empty subarray is: " << maxSubArraySum(arr) << endl;

    return 0;
}