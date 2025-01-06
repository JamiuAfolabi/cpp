#include <iostream>
#include <cassert>

using namespace std;

// Function to perform LU decomposition
void luDecomposition(double* matrix, int n, double* L, double* U) {
    // Lower triangular matrix starts at L[0*n+0] and each row is accessed by incrementing n (L[0*n+0], L[1*n+0], L[2*n+0], ...)
    // Upper triangular matrix starts at U[0*n+0] and each row is accessed by incrementing n+1 (U[0*n+0], U[0*n+1], U[1*n+0], U[1*n+1], ...)
    for (int i = 0; i < n; ++i) {
        // Upper Triangular
        for (int j = i; j < n; ++j) {
            double temp = U[i * n + j];

            for (int k = 0; k < i; ++k) {
                temp -= L[i * n + k] * U[k * n + j];
            }

            U[i * n + j] = temp;
        }

        // Lower Triangular
        for (int j = i; j < n; ++j) {
            if (i == j) {
                L[i * n + i] = 1; // Diagonal elements of L are 1
            } else {
                double temp = matrix[i * n + j];

                for (int k = 0; k < i; ++k) {
                    temp -= L[i * n + k] * U[k * n + j];
                }

                L[i * n + j] = temp / U[i * n + i];
            }
        }
    }
}

// Function to print a matrix
void printMatrix(const double* matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(10) << setprecision(4) << fixed << matrix[i * n + j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;

    cout << "Enter the size of the matrix (n x n): ";
    cin >> n;
    assert(n > 0);

    // Allocate memory for the matrix, L, and U
    double* matrix = new double[n * n];
    double* L = new double[n * n];
    double* U = new double[n * n];

    // Take input of the matrix
    cout << "Enter the elements of the matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i * n + j];
        }
    }
    
    // Copying matrix data to L
    for(int i = 0; i < n*n; i++) {
        L[i] = matrix[i];
    }

    luDecomposition(matrix, n, L, U);

    cout << "Lower triangular matrix (L):" << endl;
    printMatrix(L, n);

    cout << "\nUpper triangular matrix (U):" << endl;
    printMatrix(U, n);

    // Free the allocated memory
    delete[] matrix;
    delete[] L;
    delete[] U;

    return 0;
}