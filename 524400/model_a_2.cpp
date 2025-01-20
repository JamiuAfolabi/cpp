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
};

int main() {
    SparseMatrix matrix;

    // Inserting elements
    matrix.insert(1, 2, 5);
    matrix.insert(3, 4, 10);
    matrix.insert(2, 3, 7);
    matrix.insert(0, 1, 1);
    matrix.insert(1, 0, 3); // Add another element to row 1

    // Retrieving a value
    std::cout << "Element at (1, 2): " << matrix.get(1, 2) << std::endl;

    // Getting the sum of elements in a specific row
    std::cout << "Sum of elements in row 1: " << matrix.getRowSum(1) << std::endl;

    // Iterating through non-zero elements
    std::cout << "Non-zero elements in row-major order:" << std::endl;
    matrix.iterate();

    // Deleting an element
    matrix.erase(3, 4);
    std::cout << "After deleting element at (3, 4):" << std::endl;
    matrix.iterate();

    // Getting the sum again after deletion
    std::cout << "Sum of elements in row 1: " << matrix.getRowSum(1) << std::endl;

    return 0;
}