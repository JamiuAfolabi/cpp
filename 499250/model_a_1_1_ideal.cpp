#include<iostream>
#include<iomanip>
#include<cmath>
#include<stdlib.h>

using namespace std;

void inputMatrix(float **a, int n) {
    cout << "Enter coefficients of Matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> a[i][j];
        }
    }
}

void augmentIdentityMatrix(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j + n] = (i == j) ? 1 : 0;
        }
    }
}

bool gaussJordanElimination(float **a, int n) {
    float ratio;
    for (int i = 0; i < n; i++) {
        if (a[i][i] == 0.0) {
            cout << "Mathematical Error!" << endl;
            return false;
        }
        for (int j = 0; j < n; j++) {
            if (i != j) {
                ratio = a[j][i] / a[i][i];
                for (int k = 0; k < 2 * n; k++) {
                    a[j][k] = a[j][k] - ratio * a[i][k];
                }
            }
        }
    }
    return true;
}

void makeDiagonalOne(float **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = n; j < 2 * n; j++) {
            a[i][j] = a[i][j] / a[i][i];
        }
    }
}

void displayInverseMatrix(float **a, int n) {
    cout << endl << "Inverse Matrix is:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = n; j < 2 * n; j++) {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }
}

void deallocateMemory(float **a, int n) {
    for (int i = 0; i < n; i++) {
        delete[] a[i];
    }
    delete[] a;
}

int main() {
    int n;

    // Setting precision and writing floating point values in fixed-point notation.
    cout << setprecision(3) << fixed;

    // Input: Matrix order
    cout << "Enter order of matrix: ";
    cin >> n;

    // Dynamically allocate memory for the matrix
    float **a = new float*[n];
    for (int i = 0; i < n; i++) {
        a[i] = new float[2 * n];  // Augmented matrix will have 2*n columns
    }

    // Input: Matrix values
    inputMatrix(a, n);

    // Augmenting Identity Matrix of Order n
    augmentIdentityMatrix(a, n);

    // Applying Gauss Jordan Elimination
    if (!gaussJordanElimination(a, n)) {
        deallocateMemory(a, n);
        return 0;
    }

    // Row operation to make principal diagonal 1
    makeDiagonalOne(a, n);

    // Displaying Inverse Matrix
    displayInverseMatrix(a, n);

    // Deallocating memory
    deallocateMemory(a, n);

    return 0;
}