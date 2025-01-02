#include<iostream>
using namespace std;

int matrix_multiply(int A[][], int B[][], int C[][], int m, int k, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int l = 0; l < k; l++)
            {
                C[i][j] += A[i][l] * B[l][j];
            }
        }
    }
    return 0;
}

int main()
{
    int m, k, n;
    cout << "Enter the order of first matrix : ";
    cin >> m >> k;
    cout << "Enter the order of second matrix : ";
    cin >> k >> n;
    
    if (k != n)
    {
        cout << "Number of columns in first matrix must be equal to number of rows in second matrix." << endl;
        return 0;
    }

    int A[m][k], B[k][n], C[m][n];

    cout << "Enter the elements of first matrix : " << endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cin >> A[i][j];
        }
    }

    cout << "Enter the elements of second matrix : " << endl;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> B[i][j];
        }
    }

    matrix_multiply(A, B, C, m, k, n);

    cout << "The result of matrix multiplication is : " << endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
