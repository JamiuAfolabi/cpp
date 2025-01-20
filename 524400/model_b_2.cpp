#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

class SparseMatrix {
public:
    std::map<std::pair<int, int>, int> data;
    
    // Store row sums in a separate map for efficient lookup
    std::unordered_map<int, int> row_sums;

    void insert(int x, int y, int val) {
        if (val == 0) {
            erase(x, y);
        } else {
            data[{x, y}] = val;
            int old_val = row_sums[x];
            row_sums[x] = old_val + val;

            // If inserting the first element in a row, update the row_sums
            if (old_val == 0) {
                row_sums[x] = val;
            }
        }
    }

    void erase(int x, int y) {
        auto entry = data.find({x, y});
        if (entry != data.end()) {
            int val_to_remove = entry->second;
            data.erase(entry);

            int old_val = row_sums[x];
            row_sums[x] = old_val - val_to_remove;
            
            // If erasing the last element in a row, update the row_sums
            if (old_val == val_to_remove) {
                row_sums.erase(x);
            }
        }
    }
    
    int get(int x, int y) const {
        auto it = data.find({x, y});
        if (it != data.end()) {
            return it->second;
        }
        return 0;
    }
    int sum_row(int x) const {
        // Return row sum if it exists, otherwise 0
        auto row_sum_it = row_sums.find(x);
        return row_sum_it != row_sums.end() ? row_sum_it->second : 0;
    }

    void iterate() const {
        // Iterate through non-zero elements as before
    }
};
  
int main() {
    SparseMatrix matrix;

    matrix.insert(0, 1, 1); 
    matrix.insert(0, 2, 2);
    matrix.insert(1, 0, 3);
    matrix.insert(1, 1, 5);
    matrix.insert(1, 2, -2);
    matrix.insert(1, 3, 4);
 
    std::cout << "Sum of row 0: " << matrix.sum_row(0) << std::endl;  // Output: 3
    std::cout << "Sum of row 1: " << matrix.sum_row(1) << std::endl;  // Output: 10
    std::cout << "Sum of row 2: " << matrix.sum_row(2) << std::endl;  // Output: 0

    matrix.erase(1, 3);

    std::cout << "Sum of row 1 after erasing: " << matrix.sum_row(1) << std::endl;  // Output: 6
    return 0;
}
