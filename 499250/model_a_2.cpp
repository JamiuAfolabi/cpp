#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

int main() {
    vector<vector<float>> a;
    int n;

    // Setting precision and writing floating point values in fixed-point notation.
    cout << setprecision(3) << fixed;

    // Input
    cout << "Enter order of matrix: ";
    cin >> n;

    a.resize(n, vector<float>(2 * n, 0)); // Augmented matrix will have 2*n columns

    // Reading Matrix
    cout << "Enter coefficients of Matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> a[i][j];
        }
    }

    // Augmenting Identity Matrix
    for (int i = 0; i < n; i++) {
        a[i][i + n] = 1; // Identity matrix
    }

    // Lambda functions to simplify row operations
    auto scaleRow = [&a](int row, float scale) {
        for (size_t j = 0; j < a[row].size(); j++) {
            a[row][j] /= scale;
        }
    };

    auto addRows = [&a](int targetRow, int sourceRow, float ratio) {
        for (size_t j = 0; j < a[targetRow].size(); j++) {
            a[targetRow][j] -= ratio * a[sourceRow][j];
        }
    };

    // Applying Gauss Jordan Elimination
    for (int i = 0; i < n; i++) {
        // Check if the pivot element is zero
        if (fabs(a[i][i]) < 1e-9) {
            cout << "Mathematical Error!" << endl;
            exit(0);
        }

        // Eliminate all other rows
        for (int j = 0; j < n; j++) {
            if (i != j) {
                float ratio = a[j][i] / a[i][i];
                addRows(j, i, ratio);
            }
        }
    }

    // Scale rows to make pivot elements equal to 1
    for (int i = 0; i < n; i++) {
        scaleRow(i, a[i][i]); // Scale the i-th row by the pivot element
    }

    // Displaying Inverse Matrix
    cout << endl << "Inverse Matrix is:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = n; j < 2 * n; j++) {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}