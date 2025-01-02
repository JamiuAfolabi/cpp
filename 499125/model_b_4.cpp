#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

void multiplyMatricesOptimized(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int m, int k, int n, int threadID, int numThreads) {
    int rowSize = m / numThreads + (m % numThreads > threadID ? 1 : 0);

    for (int i = threadID * rowSize; i < min((threadID + 1) * rowSize, m); ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int r = 0; r < k; ++r) {
                C[i][j] += A[i][r] * B[r][j];
            }
        }
    }
}

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

    // Number of threads
    const int numThreads = 4; // You can adjust the number of threads based on your CPU cores

    vector<thread> threads;

    for (int threadID = 0; threadID < numThreads; ++threadID) {
        threads.push_back(thread(multiplyMatricesOptimized, A, B, C, m, k, n, threadID, numThreads));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Output the resulting matrix C
    cout << "Resulting matrix C (A * B):" << endl;
    printMatrix(C, m, n);

    return 0;
}