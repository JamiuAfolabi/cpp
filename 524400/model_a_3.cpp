#include <iostream>
#include <map>
#include <utility>

class SparseMatrix {
public:
    // Map to store the non-zero elements
    std::map<std::pair<int, int>, int> data;
    // Map to store the sum of elements for each row
    std::map<int, int> rowSums;

    // Inserts an element at (x, y) with value val
    void insert(int x, int y, int val) {
        if (val != 0) {
            int currentVal = get(x, y);
            if (currentVal != 0) {
                rowSums[x] -= currentVal; // Remove current value from the sum
            }
            data[{x, y}] = val; // Insert or update the value
            rowSums[x] += val; // Update the sum
        } else {
            int currentVal = get(x, y);
            if (currentVal != 0) {
                rowSums[x] -= currentVal; // Remove current value from the sum
            }
            data.erase({x, y}); // Remove the element if the value is zero
        }
    }

    // Deletes the element at (x, y)
    void erase(int x, int y) {
        int currentVal = get(x, y);
        if (currentVal != 0) {
            rowSums[x] -= currentVal; // Remove value from the sum
            data.erase({x, y}); // Remove the element
        }
    }

    // Retrieves the value at (x, y)
    int get(int x, int y) const {
        auto it = data.find({x, y});
        if (it != data.end()) {
            return it->second;
        }
        return 0; // Return 0 if the element doesn't exist
    }

    // Returns the sum of all elements in the given row
    int getRowSum(int x) const {
        auto it = rowSums.find(x);
        if (it != rowSums.end()) {
            return it->second;
        }
        return 0; // Return 0 if the row is empty
    }

    // Iterates through non-zero elements in row-major order
    void iterate() const {
        for (const auto& entry : data) {
            std::cout << "Element at (" << entry.first.first << ", "
                      << entry.first.second << ") = " << entry.second << std::endl;
        }
    }

    // Performs matrix multiplication with another SparseMatrix
    SparseMatrix multiply(const SparseMatrix& other) const {
        SparseMatrix result;

        // Iterate through non-zero elements of the first matrix
        for (const auto& entry1 : data) { // entry1 is of the form ((row1, col1), value1)
            int row1 = entry1.first.first;
            int col1 = entry1.first.second;
            int value1 = entry1.second;

            // For each non-zero element in the first matrix, find contributions to the result
            for (int col2 = 0; col2 < other.rowCount(); ++col2) {
                int value2 = other.get(col1, col2); // Get the corresponding element from the second matrix
                if (value2 != 0) {
                    // Update the resultant matrix at (row1, col2)
                    result.insert(row1, col2, result.get(row1, col2) + value1 * value2);
                }
            }
        }
        return result;
    }

    // Dummy method assuming we have a way to return actual row count
    // This could be implemented to store row count data while inserting
    int rowCount() const {
        return 100; // Example: replace with your actual logic to get row count
    }
};

int main() {
    SparseMatrix matrixA;
    SparseMatrix matrixB;

    // Fill matrix A
    matrixA.insert(0, 1, 3);
    matrixA.insert(1, 0, 4);
    matrixA.insert(2, 2, 5);

    // Fill matrix B
    matrixB.insert(1, 0, 2);
    matrixB.insert(0, 1, 1);
    matrixB.insert(1, 2, 3);

    // Multiply sparse matrices
    SparseMatrix result = matrixA.multiply(matrixB);

    // Output the resulting matrix
    std::cout << "Resulting matrix after multiplication:" << std::endl;
    result.iterate();

    return 0;
}