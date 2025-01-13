#include<bits/stdc++.h>
using namespace std;

int max_sum(vector<int> &arr)
{
    int n = arr.size();
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    int prefix_sum[n];
    prefix_sum[0] = arr[0];
    for (int i = 1; i < n; i++)
    {
        prefix_sum[i] = prefix_sum[i - 1] + arr[i];
    }

    int max_sum_with_rotation = 0;
    for (int i = 0; i < n; i++)
    {
        int current_sum = 0;
        int val = i * arr[i];
        current_sum += val;
        for (int j = i + 1; j < n; j++)
        {
            val += j * arr[j];
            current_sum += val;
        }
        for (int j = 0; j < i; j++)
        {
            val += j * arr[j];
            current_sum += val;
        }
        max_sum_with_rotation = max(max_sum_with_rotation, current_sum);
    }

    return max_sum_with_rotation;
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cout << "Enter size of the array: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter all the elements separated by space: ";
    for (int &x : arr)
        cin >> x;
    
    int max_sum_value = max_sum(arr);
    cout << "Maximum sum of i*arr[i] for all rotations is: " << max_sum_value << endl;

    return 0;
}