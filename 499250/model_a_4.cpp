#include <iostream>
#include <iomanip>
#include <vector>
#include <optional>
#include <cmath>
#include <cstdlib>

using namespace std;

// Function to perform Gauss-Jordan elimination and return the inverse matrix
optional<vector<vector<float>>> invertMatrix(const vector<vector<float>> &matrix) {
    int n = matrix.size();
    vector<vector<float>> a = matrix; // Make a copy of the matrix
    for (int i = 0; i < n; i++) {
        a[i].resize(2 * n, 0); // Resize for augmented matrix
        a[i][i + n] = 1; // Augment identity matrix
    }

    // Applying Gauss Jordan Elimination
    for (int i = 0; i < n; i++) {
        // Check if the pivot element is zero
        if (fabs(a[i][i]) < 1e-9) {
            return nullopt; // Matrix is singular
        }

        // Eliminate all other rows using lambda for row operations
        auto addRows = [&a](int targetRow, int sourceRow, float ratio) {
            for (size_t j = 0; j < a[targetRow].size(); j++) {
                a[targetRow][j] -= ratio * a[sourceRow][j];
            }
        };

        // Eliminate other rows
        for (int j = 0; j < n; j++) {
            if (i != j) {
                float ratio = a[j][i] / a[i][i];
                addRows(j, i, ratio);
            }
        }
    }

    // Scale rows to make pivot elements equal to 1
    for (int i = 0; i < n; i++) {
        float scale = a[i][i];
        for (size_t j = 0; j < a[i].size(); j++) {
            a[i][j] /= scale;
        }
    }

    // Extract the inverse matrix
    vector<vector<float>> inverse(n, vector<float>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse[i][j] = a[i][j + n];
        }
    }
    
    return inverse; 
}

int main() {
    int n;

    // Setting precision and writing floating-point values in fixed-point notation.
    cout << setprecision(3) << fixed;

    // Input
    cout << "Enter order of matrix: ";
    cin >> n;

    vector<vector<float>> matrix(n, vector<float>(n));

    // Reading Matrix
    cout << "Enter coefficients of Matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> matrix[i][j];
        }
    }

    // Attempt to invert the matrix
    optional<vector<vector<float>>> result = invertMatrix(matrix);

    if (result) {
        // Print the inverse matrix
        cout << endl << "Inverse Matrix is:" << endl;
        for (const auto& row : result.value()) {
            for (const auto& elem : row) {
                cout << elem << "\t";
            }
            cout << endl;
        }
    } else {
        cout << "Error: Matrix is singular or not invertible." << endl;
    }

    return 0;
}