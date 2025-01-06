#include <iostream>
using namespace std;

//Function to perform LU Decomposition
void LUDecomposition(int n, float A[n][n], float L[n][n], float U[n][n]) {
    for (int k = 0; k < n; k++) {
        L[k][k] = 1.0;
        for (int i = k+1; i < n; i++) {
            float sum = 0.0;
            for (int j = 0; j < k; j++) {
                sum += L[i][j] * U[j][k];
            }
            L[i][k] = (A[i][k] - sum) / U[k][k];
        }
        for (int j = k+1; j < n; j++) {
            float sum = 0.0;
            for (int i = 0; i < k; i++) {
                sum += L[k][i] * U[i][j];
            }
            U[k][j] = (A[k][j] - sum) / L[k][k];
        }
    }
}

// Function to display the matrix
void display(int n, float A[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
 
int main()
{
    int n;
    cout << "Enter the order of the matrix: ";
    cin >> n;

    float A[n][n];

    cout << "Enter the elements of the matrix row by row: " << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> A[i][j];
        }
    }
    cout << "Original Matrix : " << endl;
    display(n, A);
    
    float L[n][n], U[n][n];
    
    //Initialize L and U matrices with zeros
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            L[i][j] = 0;
            U[i][j] = 0;
        }
    }
    LUDecomposition(n, A, L, U);
    
    cout << "Lower Triangular Matrix (L) : " << endl;
    display(n, L);
    
    cout << "Upper Triangular Matrix (U) : " << endl;
    display(n, U);
    return 0;
}
  