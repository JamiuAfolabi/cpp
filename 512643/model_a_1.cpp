#include <iostream>
#include <vector>
#include <algorithm> // for std::find

class VectorOperations {
private:
    std::vector<int> data;

public:
    // Insert an element at the front of the vector
    void insertFront(int value) {
        data.insert(data.begin(), value);
    }

    // Insert an element at the back of the vector
    void insertBack(int value) {
        data.push_back(value);
    }

    // Search for an element in the vector
    // Returns the position of the element if found, otherwise -1
    int search(int value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            return std::distance(data.begin(), it); // Return the index of the found element
        }
        return -1; // Element not found
    }

    // Print the elements in the vector
    void print() const {
        for (int value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    VectorOperations vecOps;

    // Inserting elements
    vecOps.insertFront(10);
    vecOps.insertBack(20);
    vecOps.insertBack(30);
    vecOps.insertFront(5);

    // Print the current elements in the vector
    std::cout << "Current elements in vector: ";
    vecOps.print();

    // Searching for an element
    int searchValue = 20;
    int position = vecOps.search(searchValue);
    if (position != -1) {
        std::cout << "Element " << searchValue << " found at position: " << position << std::endl;
    } else {
        std::cout << "Element " << searchValue << " not found." << std::endl;
    }

    return 0;
}