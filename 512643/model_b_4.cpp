#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>

template <typename Container>
class ContainerOperations {
private:
    Container data;

public:
    // Insert an element at the front of the container
    void insertFront(typename Container::value_type value) {
        data.push_front(value);
    }

    // Insert an element at the back of the container
    void insertBack(typename Container::value_type value) {
        data.push_back(value);
    }

    // Search for an element in the container
    // Returns the position of the element if found, otherwise -1
    int search(typename Container::value_type value) const {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            return std::distance(data.begin(), it);
        }
        return -1;
    }

    // Print the elements in the container
    void print() const {
        for (const auto& value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

// Create template aliases for convenience
using VectorOperations = ContainerOperations<std::vector<int>>;
using DequeOperations = ContainerOperations<std::deque<int>>;
using ListOperations = ContainerOperations<std::list<int>>;

int main() {
    VectorOperations vecOps;
    DequeOperations dequeOps;
    ListOperations listOps;

    // Inserting elements
    vecOps.insertFront(1);
    vecOps.insertBack(2);
    vecOps.insertBack(3);
    vecOps.insertFront(0);

    dequeOps.insertFront(10);
    dequeOps.insertBack(20);
    dequeOps.insertBack(30);
    dequeOps.insertFront(5);
    
    listOps.insertFront(100);
    listOps.insertBack(200);
    listOps.insertBack(300);
    listOps.insertFront(50);

    // Print the current elements
    std::cout << "Vector Operations:\n";
    vecOps.print();
    std::cout << "Search 2 in Vector: " << vecOps.search(2) << std::endl;

    std::cout << "\nDeque Operations:\n";
    dequeOps.print();
    std::cout << "Search 20 in Deque: " << dequeOps.search(20) << std::endl;  

    std::cout << "\nList Operations:\n";
    listOps.print();
    std::cout << "Search 200 in List: " << listOps.search(200) << std::endl;

    return 0;
}