#include <iostream>
#include <vector>

using namespace std;

// Function to multiply two matrices
void multiplyMatricesOptimized(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int m, int k, int n) {

    // Initialize the resulting matrix C with zeros
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
        }
    }

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int temp = 0;
            for (int r = 0; r < k; r++) {
                temp += A[i][r] * B[r][j];
            }
            C[i][j] = temp;
        }
    }
}

// Function to print a matrix
void printMatrix(const vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int m, k, n;

    // Input dimensions for matrices A and B
    cout << "Enter dimensions for matrix A (m x k): ";
    cin >> m >> k;
    cout << "Enter dimensions for matrix B (k x n): ";
    cin >> k >> n; // Note that k should be the same for both matrices

    // Initialize matrices A, B, and C
    vector<vector<int>> A(m, vector<int>(k));
    vector<vector<int>> B(k, vector<int>(n));
    vector<vector<int>> C(m, vector<int>(n));

    // Input elements for matrix A
    cout << "Enter elements for matrix A:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            cin >> A[i][j];
        }
    }

    // Input elements for matrix B
    cout << "Enter elements for matrix B:" << endl;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> B[i][j];
        }
    }

    // Multiply matrices A and B, store result in C
    multiplyMatricesOptimized(A, B, C, m, k, n);

    // Output the resulting matrix C
    cout << "Resulting matrix C (A * B):" << endl;
    printMatrix(C, m, n);

    return 0;
}