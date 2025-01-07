#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

// Function to calculate the sum of a k x k submatrix starting at (row, col)
int sumSubmatrix(const vector<vector<int>>& matrix, int row, int col, int k) {
    int sum = 0;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            sum += matrix[row + i][col + j];
        }
    }
    return sum;
}

int main() {
    int N;
    cout << "Enter the size of the matrix (N x N): ";
    cin >> N;

    vector<vector<int>> matrix(N, vector<int>(N));
    cout << "Enter the elements of the matrix:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> matrix[i][j];
        }
    }

    int maxSum = INT_MIN;
    int maxRow = 0, maxCol = 0, maxK = 1;

    // Iterate through all possible sizes of submatrices
    for (int k = 1; k <= N; ++k) {
        // Iterate through all possible starting positions for the submatrix
        for (int i = 0; i <= N - k; ++i) {
            for (int j = 0; j <= N - k; ++j) {
                int currentSum = sumSubmatrix(matrix, i, j, k);
                if (currentSum > maxSum) {
                    maxSum = currentSum;
                    maxRow = i;
                    maxCol = j;
                    maxK = k;
                }
            }
        }
    }

    cout << "Maximum sum submatrix found at (" << maxRow << ", " << maxCol << ") with size " << maxK << " x " << maxK << " and sum = " << maxSum << endl;
    cout << "Submatrix:" << endl;
    for (int i = 0; i < maxK; ++i) {
        for (int j = 0; j < maxK; ++j) {
            cout << matrix[maxRow + i][maxCol + j] << " ";
        }
        cout << endl;
    }

    return 0;
}