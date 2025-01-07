#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

int sumSubmatrix(const vector<vector<int>>& matrix, int row, int col, int k) {
    if (row == matrix.size() - k || col == matrix[row].size() - k) {
        return 0;
    }

    int sum = matrix[row][col];

    for (int i = 1; i < k; ++i) {
        sum += matrix[row + i][col];
        sum += matrix[row][col + i];
        sum += matrix[row + i][col + i];
    }

    for (int i = 1; i < k; ++i) {
        sum += matrix[row + k - 1][col + i];
        sum += matrix[row + i][col + k - 1];
    }

    return sum;
}

int maxSumKxKSumMatrix(const vector<vector<int>>& matrix, int k) {
    int maxSum = INT_MIN;
    for (int i = 0; i <= matrix.size() - k; ++i) {
        for (int j = 0; j <= matrix[i].size() - k; ++j) {
            maxSum = max(maxSum, sumSubmatrix(matrix, i, j, k));
        }
    }
    return maxSum;
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
    int maxK = 1;
    for (int k = 1; k <= N; ++k) {
        int sum = maxSumKxKSumMatrix(matrix, k);
        if (sum > maxSum) {
            maxSum = sum;
            maxK = k;
        }
    }

    cout << "Maximum sum submatrix size = " << maxK << " x " << maxK << " and sum = " << maxSum << endl;

    return 0;
}