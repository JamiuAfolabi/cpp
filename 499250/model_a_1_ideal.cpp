#include<iostream>
#include<iomanip>
#include<cmath>
#include<stdlib.h>

using namespace std;

int main() {
    float **a, ratio;
    int i, j, k, n;

    /* Setting precision and writing floating point values in fixed-point notation. */
    cout << setprecision(3) << fixed;

    /* Inputs */
    /* 1. Reading order of matrix */
    cout << "Enter order of matrix: ";
    cin >> n;

    // Dynamically allocate memory for the matrix
    a = new float*[n];
    for (i = 0; i < n; i++) {
        a[i] = new float[2 * n];  // Augmented matrix will have 2*n columns
    }

    /* 2. Reading Matrix */
    cout << "Enter coefficients of Matrix: " << endl;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> a[i][j];
        }
    }

    /* Augmenting Identity Matrix of Order n */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                a[i][j + n] = 1;
            } else {
                a[i][j + n] = 0;
            }
        }
    }

    /* Applying Gauss Jordan Elimination */
    for (i = 0; i < n; i++) {
        if (a[i][i] == 0.0) {
            cout << "Mathematical Error!" << endl;
            exit(0);
        }
        for (j = 0; j < n; j++) {
            if (i != j) {
                ratio = a[j][i] / a[i][i];
                for (k = 0; k < 2 * n; k++) {
                    a[j][k] = a[j][k] - ratio * a[i][k];
                }
            }
        }
    }

    /* Row Operation to Make Principal Diagonal to 1 */
    for (i = 0; i < n; i++) {
        for (j = n; j < 2 * n; j++) {
            a[i][j] = a[i][j] / a[i][i];
        }
    }

    /* Displaying Inverse Matrix */
    cout << endl << "Inverse Matrix is:" << endl;
    for (i = 0; i < n; i++) {
        for (j = n; j < 2 * n; j++) {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }

    // Deallocating memory
    for (i = 0; i < n; i++) {
        delete[] a[i];
    }
    delete[] a;

    return 0;
}
