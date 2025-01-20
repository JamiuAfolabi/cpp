
#include <map>
#include <iterator>
#include <tuple>
#include <iostream>

class SparseMatrix {
private:
    using Element = std::pair<int, int>; // (y, value)
    using RowMap = std::map<int, std::map<int, int>>;

    RowMap data;

public:
    // Insert an element at (x, y) with value val.
    void insert(int x, int y, int val) {
        if (val == 0) {
            data[x].erase(y);
        } else {
            data[x][y] = val;
        }
    }

    // Retrieve the value at (x, y). Returns 0 if the element doesn't exist.
    int getValue(int x, int y) const {
        auto it = data.find(x);
        if (it != data.end() && it->second.find(y) != it->second.end()) {
            return it->second.at(y);
        }
        return 0;
    }

    // Remove the element at (x, y).
    void remove(int x, int y) {
        data[x].erase(y);
    }

    // Iterate through non-zero elements in row-major order using a range-based for loop
    class RowMajorIterator {
    public:
        using value_type = std::tuple<int, int, int>; // (x, y, value)

        RowMajorIterator(const RowMap& data) : current(data.begin()) {}
        RowMajorIterator& operator++() {
            while (current != data.end()) {
                auto row_it = current->second.begin();
                while (row_it != current->second.end()) {
                    ++row_it;
                }
                ++current;
            }
            return *this;
        }
        bool operator!=(const RowMajorIterator& other) const {
            return current != other.current;
        }
        value_type operator*() const {
            return {current->first, current->second.begin()->first, current->second.begin()->second};
        }

    private:
        RowMap::const_iterator current;
    };

    friend class RowMajorIterator;

    auto begin() const { return RowMajorIterator(data); }
    auto end() const { return RowMajorIterator{}; }
};


int main() {
    SparseMatrix matrix;
    matrix.insert(0, 0, 1);
    matrix.insert(0, 1, 2);
    matrix.insert(1, 0, 3);
    matrix.insert(1, 2, 4);

    for (auto& [x, y, val] : matrix) {
        std::cout << "(x, y, value) = (" << x << ", " << y << ", " << val << ")\n";
    }

    std::cout << "Value at (0, 0): " << matrix.getValue(0, 0) << "\n";
    matrix.remove(0, 1);

    return 0;
}

 