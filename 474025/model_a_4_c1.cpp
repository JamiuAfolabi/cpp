#include <iostream>

int main() {
    int x = -1;
    if (x < 0) {
        std::cerr << "Error: x must be a non-negative integer." << std::endl;
    }
    return 0;
}