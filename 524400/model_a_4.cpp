#include <iostream>
#include <map>
#include <utility>
#include <vector>

class SparseMatrix {
public:
    std::map<std::pair<int, int>, int> data; // Store non-zero elements as (row, col) -> value
    std::map<int, int> rowSums; // Track sums of each row

    void insert(int x, int y, int val) {
        if (val != 0) {
            int currentVal = get(x, y);
            if (currentVal != 0) {
                rowSums[x] -= currentVal; // Remove current value from the sum
            }
            data[{x, y}] = val; // Insert or update the value
            rowSums[x] += val; // Add the new value to the sum
        } else {
            int currentVal = get(x, y);
            if (currentVal != 0) {
                rowSums[x] -= currentVal; // Remove current value from the sum
            }
            data.erase({x, y}); // Remove the element if the value is zero
        }
    }

    int get(int x, int y) const {
        auto it = data.find({x, y});
        return (it != data.end()) ? it->second : 0;
    }

    // Method to find non-zero elements within the rectangle defined by (x1, y1) and (x2, y2)
    std::vector<std::pair<std::pair<int, int>, int>> rangeQuery(int x1, int y1, int x2, int y2) const {
        std::vector<std::pair<std::pair<int, int>, int>> results;

        // Iterate through each relevant key in the data map
        for (const auto& entry : data) {
            int row = entry.first.first;
            int col = entry.first.second;

            // Check if the element is within the defined rectangle
            if (row >= x1 && row <= x2 && col >= y1 && col <= y2) {
                results.emplace_back(entry.first, entry.second);
            }
        }

        return results;
    }

    void print() const {
        for (const auto& entry : data) {
            std::cout << "Element at (" << entry.first.first << ", " << entry.first.second << ") = " << entry.second << std::endl;
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
    matrix.insert(1, 0, 3);
    matrix.insert(4, 4, 6);

    // Print the current matrix
    std::cout << "Current Sparse Matrix:" << std::endl;
    matrix.print();

    // Range query example
    int x1 = 1, y1 = 0, x2 = 3, y2 = 4;
    std::cout << "Non-zero elements in the rectangle defined by (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << "):" << std::endl;
    auto results = matrix.rangeQuery(x1, y1, x2, y2);

    for (const auto& result : results) {
        std::cout << "Element at (" << result.first.first << ", " << result.first.second << ") = " << result.second << std::endl;
    }

    return 0;
}