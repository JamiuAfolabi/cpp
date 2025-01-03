#include<bits/stdc++.h>
using namespace std;

// function to multiply two matrices
void matrix_multiply(int mat1[2][3], int mat2[3][2], int result[2][2])
{
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; j < 2; j++)
      {
         for (int k = 0; k < 3; k++)
         {
            result[i][j] += mat1[i][k] * mat2[k][j];
         }
      }
   }
}
 
int main()
{
   int matrix[2][3] = {{1, 2, 3},
                        {4, 5, 6}};
 
   cout << "Given Matrix: \n";
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         cout << matrix[i][j] << " ";
      }
      cout << endl;
   }

   int result[2][2]; // Result matrix

   // Since the number of columns in the first matrix should be equal to the number of rows in the second matrix, we can multiply these matrices
   // The size of the result matrix will be (rows in the first matrix) * (columns in the second matrix)
   matrix_multiply(matrix, matrix, result);

   cout << "\nResult of A*A: \n";
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; j < 2; j++)
      {
         cout << result[i][j] << " ";
      }
      cout << endl;
   }
   return 0;
}
