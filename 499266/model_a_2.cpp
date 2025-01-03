#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// Function to display a matrix
void displayMatrix(const shared_ptr<vector<vector<int>>>& matrix) {
    for (const auto& row : *matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Function to multiply two matrices
shared_ptr<vector<vector<int>>> multiplyMatrices(const shared_ptr<vector<vector<int>>>& A,
                                                 const shared_ptr<vector<vector<int>>>& B) {
    int rowsA = A->size();
    int colsA = (*A)[0].size();
    int colsB = (*B)[0].size();
    
    // Create a new matrix for the result using shared_ptr
    auto result = make_shared<vector<vector<int>>>(rowsA, vector<int>(colsB, 0));
    
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) { // or colsB, since colsA == rowsB
                (*result)[i][j] += (*A)[i][k] * (*B)[k][j];
            }
        }
    }
    
    return result; // Return shared_ptr to the result matrix
}

int main() {
    // Initialize matrix A using shared_ptr
    auto A = make_shared<vector<vector<int>>>(vector<vector<int>>{
        {1, 2},
        {3, 4}
    });

    // Print the original matrix A
    cout << "Matrix A:" << endl;
    displayMatrix(A);

    // Carry out multiplication A * A
    auto result = multiplyMatrices(A, A);

    // Print the result of A * A
    cout << "Result of A * A:" << endl;
    displayMatrix(result);
    
    return 0;
}