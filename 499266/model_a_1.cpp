#include <iostream>
#include <vector>

using namespace std;

// Function to display a matrix
void displayMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Function to multiply two matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();
    
    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));
    
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) { // or colsB, since colsA == rowsB
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return result;
}

int main() {
    // Initialize matrix A
    vector<vector<int>> A = {
        {1, 2},
        {3, 4}
    };

    // Print the original matrix A
    cout << "Matrix A:" << endl;
    displayMatrix(A);

    // Carry out multiplication A * A
    vector<vector<int>> result = multiplyMatrices(A, A);

    // Print the result of A * A
    cout << "Result of A * A:" << endl;
    displayMatrix(result);
    
    return 0;
}