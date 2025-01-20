#include <iostream>
#include <map>
#include <utility>

class SparseMatrix {
public:
    // Using a map to store non-zero elements, where:
    // key is a pair of (row, column) and value is the element
    std::map<std::pair<int, int>, int> data;
    // Map to store the sum of elements for each row
    std::map<int, int> rowSums;

    // Inserts an element at (x, y) with value val
    void insert(int x, int y, int val) {
        if (val != 0) {
            // Update the row sum
            int currentVal = get(x, y);
            if (currentVal != 0) {
                rowSums[x] -= currentVal; // Remove current value from the sum
            }
            data[{x, y}] = val; // Insert or update the value
            rowSums[x] += val; // Add the new value to the sum
        } else {
            // If the value is zero, erase the element and update the sum
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

    SparseMatrix multiply(const SparseMatrix& other) const {
        SparseMatrix result;
        int colsInThis = data.empty() ? 0 : data.rbegin()->first.second + 1;
        int rowsInOther = other.data.empty() ? 0 : other.data.rbegin()->first.first + 1;

        // Check if the number of columns in this matrix is equal to the number of rows in the other matrix
        if (colsInThis != rowsInOther) {
            std::cout << "Number of columns in this matrix must equal number of rows in the other matrix." << std::endl;
            return result;
        }

        // Iterate through the more sparse matrix to reduce loop iterations
        size_t sparse_size = (data.size() < other.data.size()) ? data.size() : other.data.size();

        for (const auto& this_entry : data) {
            int rowThis = this_entry.first.first;
            int colThis = this_entry.first.second;
            int valThis = this_entry.second;

            for (const auto& other_entry : other.data) {
                int colOther = other_entry.first.second;
                int valOther = other_entry.second;

                if (colThis == other_entry.first.first) {
                    int key = rowThis * colsInThis + colOther;
                    result.insert(rowThis, colOther, valThis * valOther);
                }
            }
        }

        return result;
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
    matrixB.insert(2, 1, 3);

    // Multiply sparse matrices
    SparseMatrix result = matrixA.multiply(matrixB);

    // Output the resulting matrix
    std::cout << "Resulting matrix after multiplication:" << std::endl;
    result.iterate();

    return 0;
}