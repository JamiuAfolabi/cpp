#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void printMatrix(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(10) << setprecision(5) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

bool invertMatrix(vector<vector<double>>& matrix, vector<vector<double>>& inverse) {
    int n = matrix.size();
    // Create an augmented matrix with the identity matrix
    inverse.resize(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++) {
        inverse[i][i] = 1;
    }

    // Forward elimination
    for (int i = 0; i < n; i++) {
        // Find pivot
        double pivot = matrix[i][i];
        if (abs(pivot) < 1e-12) { // check for singularity
            cout << "Matrix is singular, cannot compute its inverse." << endl;
            return false;
        }

        // Normalize the pivot row
        for (int j = 0; j < n; j++) {
            matrix[i][j] /= pivot;
            inverse[i][j] /= pivot;
        }

        // Eliminate other rows
        for (int j = 0; j < n; j++) {
            if (j != i) {
                double multiplier = matrix[j][i];
                for (int k = 0; k < n; k++) {
                    matrix[j][k] -= multiplier * matrix[i][k];
                    inverse[j][k] -= multiplier * inverse[i][k];
                }
            }
        }
    }
    return true;
}

int main() {
    int n;
    cout << "Enter the size of the matrix (n x n): ";
    cin >> n;

    vector<vector<double>> matrix(n, vector<double>(n));
    cout << "Enter the elements of the matrix (row by row):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    vector<vector<double>> inverse;
    if (invertMatrix(matrix, inverse)) {
        cout << "Inverse of the matrix is:" << endl;
        printMatrix(inverse);
    }

    return 0;
}