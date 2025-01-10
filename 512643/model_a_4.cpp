#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <type_traits>

template <typename Container>
class ContainerOperations {
    static_assert(std::is_default_constructible<Container>::value, "Container must be default constructible");

private:
    Container data;

public:
    // Insert an element at the front of the container
    void insertFront(typename Container::value_type value) {
        data.insert(data.begin(), value);
    }

    // Insert an element at the back of the container
    void insertBack(typename Container::value_type value) {
        data.push_back(value);
    }

    // Search for an element in the container
    int search(typename Container::value_type value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            return std::distance(data.begin(), it); // Return the index of the found element
        }
        return -1; // Element not found
    }

    // Print the elements in the container
    void print() const {
        for (const auto& value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Testing with std::vector
    std::cout << "Testing with std::vector:\n";
    ContainerOperations<std::vector<int>> vecOps;
    vecOps.insertBack(10);
    vecOps.insertFront(20);
    vecOps.insertBack(30);
    vecOps.print();
    std::cout << "Search for 20: " << vecOps.search(20) << std::endl;

    // Testing with std::deque
    std::cout << "\nTesting with std::deque:\n";
    ContainerOperations<std::deque<int>> deqOps;
    deqOps.insertBack(5);
    deqOps.insertFront(15);
    deqOps.insertBack(25);
    deqOps.print();
    std::cout << "Search for 15: " << deqOps.search(15) << std::endl;

    // Testing with std::list
    std::cout << "\nTesting with std::list:\n";
    ContainerOperations<std::list<int>> listOps;
    listOps.insertBack(100);
    listOps.insertFront(200);
    listOps.insertBack(300);
    listOps.print();
    std::cout << "Search for 200: " << listOps.search(200) << std::endl;

    return 0;
}