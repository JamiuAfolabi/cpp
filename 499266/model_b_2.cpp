#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// Function to display a matrix
void displayMatrix(const shared_ptr<vector<vector<int>>>& matrix) {
    if (matrix) {
        for (const auto& row : *matrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Matrix is empty." << endl;
    }
}

// Function to multiply two matrices
shared_ptr<vector<vector<int>>> multiplyMatrices(const shared_ptr<vector<vector<int>>>& A, const shared_ptr<vector<vector<int>>>& B) {
    int rowsA = (*A)[0].size();
    int colsA = A->size();
    int colsB = (*B)[0].size();
    
    if (colsA != rowsB) {
        cout << "Number of columns in A must equal number of rows in B." << endl;
        return nullptr;
    }

    shared_ptr<vector<vector<int>>> result = make_shared<vector<vector<int>>>(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                (*result)[i][j] += (*A)[i][k] * (*B)[k][j];
            }
        }
    }

    return result;
}

int main() {
    // Initialize matrix A
    auto A = make_shared<vector<vector<int>>>(2, vector<int>(2));
    A->at(0) = { 1, 2 };
    A->at(1) = { 3, 4 };

    // Print the original matrix A
    cout << "Matrix A:" << endl;
    displayMatrix(A);

    // Carry out multiplication A * A
    auto result = multiplyMatrices(A, A);

    // Print the result of A * A
    cout << endl << "Result of A * A:" << endl;
    displayMatrix(result);
    
    return 0;
}