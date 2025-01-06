#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// Function to perform LU decomposition
void luDecomposition(const vector<vector<double>>& matrix, vector<vector<double>>& L, vector<vector<double>>& U, int n) {
    for (int i = 0; i < n; ++i) {
        // Upper Triangular
        for (int j = i; j < n; ++j) {
            U[i][j] = matrix[i][j]; // U[i][j] = matrix[i][j]

            for (int k = 0; k < i; ++k) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        
        // Lower Triangular
        for (int j = i; j < n; ++j) {
            if (i == j) {
                L[i][i] = 1; // Diagonal elements of L are 1
            } else {
                L[j][i] = matrix[j][i]; // L[j][i] = matrix[j][i]

                for (int k = 0; k < i; ++k) {
                    L[j][i] -= L[j][k] * U[k][i];
                }

                L[j][i] /= U[i][i];
            }
        }
    }
}

// Function to print a matrix
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double item : row) {
            cout << setw(10) << setprecision(4) << fixed << item << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;

    cout << "Enter the size of the matrix (n x n): ";
    cin >> n;

    // Create matrices with smart pointers (using std::vector)
    vector<vector<double>> matrix(n, vector<double>(n));
    vector<vector<double>> L(n, vector<double>(n, 0.0)); // Lower triangular matrix
    vector<vector<double>> U(n, vector<double>(n, 0.0)); // Upper triangular matrix

    cout << "Enter the elements of the matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
        }
    }

    luDecomposition(matrix, L, U, n);

    cout << "Lower triangular matrix (L):" << endl;
    printMatrix(L);

    cout << "Upper triangular matrix (U):" << endl;
    printMatrix(U);

    return 0;
}