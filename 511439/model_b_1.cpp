#include <iostream>
using namespace std;

int lengthOfLIS(int arr[], int n)
{
   if (n == 0)
      return 0;

   int dp[n] = {1};

   for (int i = 1; i < n; i++)
   {
      for (int j = 0; j < i; j++)
      {
         if (arr[i] > arr[j])
            dp[i] = max(dp[i], dp[j] + 1);
      }
   }

   return *max_element(dp, dp+n);
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    int arr[n];
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    
    cout << "Length of LIS is: " << lengthOfLIS(arr, n) << endl;

    return 0;
}