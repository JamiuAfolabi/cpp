#include <iostream>
#include <vector>
#include <limits.h>
#include <limits>

using namespace std;

// Function prototypes
int sumSubmatrixRecursive(const vector<vector<int>>& matrix, int row, int col, int k, int i = 0, int j = 0);

// Recursive function to calculate the sum of a k x k submatrix starting at (row, col)
int sumSubmatrixRecursive(const vector<vector<int>>& matrix, int row, int col, int k, int i = 0, int j = 0) {
    if (i == k) {
        return 0;  // Base case: no more rows to add
    }
    if (j == k) {
        return sumSubmatrixRecursive(matrix, row, col, k, i + 1, 0);  // Move to the next row
    }
    return matrix[row + i][col + j] + sumSubmatrixRecursive(matrix, row, col, k, i, j + 1);  // Add current element and move to the next column
}

int main() {
    int N;
    
    // Input size validation
    while (true) {
        cout << "Enter the size of the matrix (N x N, N should be a positive integer): ";
        cin >> N;

        if (cin.fail() || N <= 0) {
            cin.clear(); // Clear the input error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
            cout << "Invalid input. Please enter a positive integer." << endl;
        } else {
            break; // Valid input for N, exit the loop
        }
    }

    vector<vector<int>> matrix(N, vector<int>(N));
    
    cout << "Enter the elements of the matrix (integers):" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {

            while (true) {
                cout << "Element [" << i << "][" << j << "]: ";
                cin >> matrix[i][j];

                if (cin.fail()) {
                    cin.clear(); // Clear the input error state
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                    cout << "Invalid input. Please enter an integer." << endl;
                } else {
                    break; // Valid input, exit the loop
                }
            }
        }
    }

    int maxSum = INT_MIN;
    int maxRow = 0, maxCol = 0, maxK = 1;

    // Iterate through all possible sizes of submatrices
    for (int k = 1; k <= N; ++k) {
        // Iterate through all possible starting positions for the submatrix
        for (int i = 0; i <= N - k; ++i) {
            for (int j = 0; j <= N - k; ++j) {
                int currentSum = sumSubmatrixRecursive(matrix, i, j, k);
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