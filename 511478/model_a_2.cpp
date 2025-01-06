#include <iostream>
#include <iomanip>

using namespace std;

// Function to perform LU decomposition using pointers
void luDecomposition(double** matrix, double** L, double** U, int n) {
    for (int i = 0; i < n; ++i) {
        // Upper Triangular
        for (int j = i; j < n; ++j) {
            *(*(U + i) + j) = *(*(matrix + i) + j); // U[i][j] = matrix[i][j]

            for (int k = 0; k < i; ++k) {
                *(*(U + i) + j) -= *(*(L + i) + k) * (*(*(U + k) + j));
            }
        }
        
        // Lower Triangular
        for (int j = i; j < n; ++j) {
            if (i == j) {
                *(*(L + i) + i) = 1; // Diagonal elements of L are 1
            } else {
                *(*(L + j) + i) = *(*(matrix + j) + i); // L[j][i] = matrix[j][i]

                for (int k = 0; k < i; ++k) {
                    *(*(L + j) + i) -= *(*(L + j) + k) * (*(*(U + k) + i));
                }

                *(*(L + j) + i) /= *(*(U + i) + i);
            }
        }
    }
}

// Function to print a matrix using pointers
void printMatrix(double** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(10) << setprecision(4) << fixed << *(*(matrix + i) + j) << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;

    cout << "Enter the size of the matrix (n x n): ";
    cin >> n;

    // Allocate memory for the matrices
    double** matrix = new double*[n];
    double** L = new double*[n];
    double** U = new double*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new double[n];
        L[i] = new double[n]();
        U[i] = new double[n]();
    }

    cout << "Enter the elements of the matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> *(*(matrix + i) + j);
        }
    }

    luDecomposition(matrix, L, U, n);

    cout << "Lower triangular matrix (L):" << endl;
    printMatrix(L, n);

    cout << "Upper triangular matrix (U):" << endl;
    printMatrix(U, n);

    // Free allocated memory
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
        delete[] L[i];
        delete[] U[i];
    }
    delete[] matrix;
    delete[] L;
    delete[] U;

    return 0;
}