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

shared_ptr<vector<vector<int>>> multiplyMatrices(const shared_ptr<vector<vector<int>>>& A,
                                                 const shared_ptr<vector<vector<int>>>& B) {

    if (A == nullptr || B == nullptr) {
        throw runtime_error("Cannot multiply matrices: Null pointer input.");
    }

    int rowsA = A->size();
    int colsA = (*A)[0].size();
    int rowsB = B->size();
    int colsB = (*B)[0].size();

    if (colsA != rowsB) {
        throw runtime_error("Cannot multiply matrices: Mismatched inner dimensions.");
    }

    // Create a new matrix for the result using shared_ptr
    auto result = make_shared<vector<vector<int>>>(rowsA, vector<int>(colsB, 0));
    
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
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
    auto B = make_shared<vector<vector<int>>>(vector<vector<int>>{
        {1, 2},
        {3, 4}
    });

    try {
        // Printing A
        cout << "Matrix A:" << endl;
        displayMatrix(A); 

        // Multiplying A and B
        auto result = multiplyMatrices(A, B);

        // Printing the result of A * B
        cout << "Result of A * B:" << endl;
        displayMatrix(result);

    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}