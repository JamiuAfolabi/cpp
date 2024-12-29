#include <array>
#include <vector>
#include <iostream>

const int SIZE = 1000;

int main() {
    // Using std::array
    std::array<int, SIZE> myArray; // Fixed size array
    for (int i = 0; i < SIZE; ++i) {
        myArray[i] = i; // Accessing elements
    }
    
    // Using std::vector
    std::vector<int> myVector; // Dynamic array
    myVector.reserve(SIZE); // Reserve space
    for (int i = 0; i < SIZE; ++i) {
        myVector.push_back(i); // Dynamically adding elements
    }
    
    // Accessing elements
    for (int i = 0; i < SIZE; ++i) {
        std::cout << myArray[i] << " " << myVector[i] << "\n"; // Should print same values
    }
    
    return 0;
}