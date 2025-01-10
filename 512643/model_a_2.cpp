#include <iostream>
#include <deque>
#include <algorithm> // for std::find

class DequeOperations {
private:
    std::deque<int> data;

public:
    // Insert an element at the front of the deque
    void insertFront(int value) {
        data.push_front(value);
    }

    // Insert an element at the back of the deque
    void insertBack(int value) {
        data.push_back(value);
    }

    // Search for an element in the deque
    // Returns the position of the element if found, otherwise -1
    int search(int value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            return std::distance(data.begin(), it); // Return the index of the found element
        }
        return -1; // Element not found
    }

    // Print the elements in the deque
    void print() const {
        for (int value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    DequeOperations dequeOps;

    // Inserting elements
    dequeOps.insertFront(10);
    dequeOps.insertBack(20);
    dequeOps.insertBack(30);
    dequeOps.insertFront(5);

    // Print the current elements in the deque
    std::cout << "Current elements in deque: ";
    dequeOps.print();

    // Searching for an element
    int searchValue = 20;
    int position = dequeOps.search(searchValue);
    if (position != -1) {
        std::cout << "Element " << searchValue << " found at position: " << position << std::endl;
    } else {
        std::cout << "Element " << searchValue << " not found." << std::endl;
    }

    return 0;
}