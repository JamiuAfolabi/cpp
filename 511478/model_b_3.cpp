#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

// Function to perform LU decomposition using unique_ptr
void luDecomposition(unique_ptr<unique_ptr<double>[]>& matrix, unique_ptr<unique_ptr<double>[]>& L, unique_ptr<unique_ptr<double>[]>& U, int n) {
    for (int i = 0; i < n; ++i) {
        // Upper Triangular
        for (int j = i; j < n; ++j) {
            U[i][j] = matrix[i][j];

            for (int k = 0; k < i; ++k) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        
        // Lower Triangular
        for (int j = i; j < n; ++j) {
            if (i == j) L[i][i] = 1; // Diagonal elements of L are 1
            else {
                L[j][i] = matrix[j][i];

                for (int k = 0; k < i; ++k) {
                    L[j][i] -= L[j][k] * U[k][i];
                }

                L[j][i] /= U[i][i];
            }
        }
    }
}

// Function to print a matrix using unique_ptr
void printMatrix(const unique_ptr<unique_ptr<double>[]>& matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(10) << setprecision(4) << fixed << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;

    cout << "Enter the size of the matrix (n x n): ";
    cin >> n;

    // Allocate memory for the matrices using unique_ptr
    unique_ptr<unique_ptr<double>[]> matrix(new unique_ptr<double>[n]);
    unique_ptr<unique_ptr<double>[]> L(new unique_ptr<double>[n]);
    unique_ptr<unique_ptr<double>[]> U(new unique_ptr<double>[n]);

    for (int i = 0; i < n; ++i) {
        matrix[i] = make_unique<double[]>(n);
        L[i] = make_unique<double[]>(n);
        U[i] = make_unique<double[]>(n);
    }

    cout << "Enter the elements of the matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
        }
    }

    luDecomposition(matrix, L, U, n);

    cout << "Lower triangular matrix (L):" << endl;
    printMatrix(L, n);

    cout << "Upper triangular matrix (U):" << endl;
    printMatrix(U, n);

    return 0;
}