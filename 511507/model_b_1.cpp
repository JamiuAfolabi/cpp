#include<bits/stdc++.h>
using namespace std;

int max_subarray_sum(vector<vector<int>>& mat, int rows, int cols)
{
   for (int i = 1; i < rows; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         mat[i][j] += max(mat[i-1][j], 0);
      }
   }

   int max_sum = mat[0][0];
   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         max_sum = max(max_sum, mat[i][j]);
      }
   }
   return max_sum;
}

void max_submatrix(vector<vector<int>>& mat, int rows, int cols, int k)
{
   int max_sum = INT_MIN;
   vector<vector<int>> cur_sum(rows, vector<int> (cols, 0));
   int rStart = 0;

   for (int i = 0; i < k; i++)
   {
      for (int j = 0; j < k; j++)
      {
         cur_sum[i][j] = mat[i][j];
      }
   }
   max_sum = max(max_sum, max_subarray_sum(cur_sum, k, k));
   int len = k - 1;
   for (int i = k; i < rows; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         cur_sum[i][j] = cur_sum[i - 1][j] + mat[i][j];
      }
      for (int j = 0; j < k; j++)
      { 
         max_sum = max(max_sum, max_subarray_sum(cur_sum, k, k));
         cur_sum[i][j] -= mat[i - len][j];
      }
      cur_sum[i][k - 1] = INT_MIN;
      len++;
   }
   cout << "Maximum sum of the submatrix: " << max_sum << endl;
   if(max_sum != INT_MIN)
   {
      for (int i = k; i > k - len && i >= 0; i--)
      {
         if (max_sum == max_subarray_sum(cur_sum, k, k))
         {
            for (int j = 0; j < k; j++)
            {
               cout << " " << cur_sum[i][j];
            }
            cout << endl;
            for (int j = k; j > 0; j--)
            {
               if (max_sum == max_subarray_sum(cur_sum, k, k))
               {
                  cout << "[" << i - k + 1 << ", " << j << "] => ";
                  for (int k = j; k >= 0; k--)
                  {
                     cout << " " << mat[i][k];
                  }
                  cout << endl;
               }
               cur_sum[i][j - 1] -= mat[i][j - 1];
            }
            cout << endl;
            break;
         }
      }
   }

}
 
 int main()
 {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int N, k;
  cout << "Enter the size of the matrix (N x N): ";
  cin >> N;
  cout << "Enter the size of the submatrix (k x k) : ";
  cin >> k;

  if(k<=0 || k>N)
  {
      cout << "Invalid value of k! k should be between 1 and N." << endl;
      return 0;
  }

  vector<vector<int>> mat(N, vector<int> (N, 0));