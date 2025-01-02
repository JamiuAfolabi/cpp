#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// Function to multiply a portion of the matrices
void multiplyMatrixPart(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int startRow, int endRow, int k, int n) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;  // Initialize C[i][j]
            for (int r = 0; r < k; ++r) {
                C[i][j] += A[i][r] * B[r][j];
            }
        }
    }
}

// Function to multiply two matrices using multithreading
void multiplyMatricesMultiThreaded(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int m, int k, int n) {
    // Number of threads to use
    int numThreads = thread::hardware_concurrency();
    vector<thread> threads;

    // Calculate the number of rows each thread will process
    int rowsPerThread = m / numThreads;
    int leftoverRows = m % numThreads;

    int startRow = 0;

    for (int i = 0; i < numThreads; ++i) {
        int endRow = startRow + rowsPerThread + (i < leftoverRows ? 1 : 0); // Distribute leftover rows
        threads.push_back(thread(multiplyMatrixPart, ref(A), ref(B), ref(C), startRow, endRow, k, n));
        startRow = endRow;  // Update startRow for the next thread
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
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
    multiplyMatricesMultiThreaded(A, B, C, m, k, n);

    // Output the resulting matrix C
    cout << "Resulting matrix C (A * B):" << endl;
    printMatrix(C, m, n);

    return 0;
}