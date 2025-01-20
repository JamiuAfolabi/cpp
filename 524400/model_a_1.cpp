#include <iostream>
#include <map>
#include <tuple>

class SparseMatrix {
public:
    // Using a map to store non-zero elements, where:
    // key is a pair of (row, column) and value is the element
    std::map<std::pair<int, int>, int> data;

    // Inserts an element at (x, y) with value val
    void insert(int x, int y, int val) {
        if (val != 0) {
            data[{x, y}] = val; // Insert or update the value
        } else {
            data.erase({x, y}); // Remove the element if the value is zero
        }
    }

    // Deletes the element at (x, y)
    void erase(int x, int y) {
        data.erase({x, y});
    }

    // Retrieves the value at (x, y)
    int get(int x, int y) const {
        auto it = data.find({x, y});
        if (it != data.end()) {
            return it->second;
        }
        return 0; // Return 0 if the element doesn't exist
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

    // Retrieving an element
    std::cout << "Element at (1, 2): " << matrix.get(1, 2) << std::endl;

    // Iterating through non-zero elements
    std::cout << "Non-zero elements in row-major order:" << std::endl;
    matrix.iterate();

    // Deleting an element
    matrix.erase(3, 4);
    std::cout << "After deleting element at (3, 4):" << std::endl;
    matrix.iterate();

    return 0;
}