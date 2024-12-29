#include <iostream>

int main() {
    int value = 2;

    switch (value) {
        case 1:
            std::cout << "Value is 1" << std::endl;
            break;
        case 2:
            std::cout << "Value is 2" << std::endl;
            break;
        case 3:
            std::cout << "Value is 3" << std::endl;
            break;
        default:
            std::cout << "Value is not 1, 2, or 3" << std::endl;
            break;
    }

    return 0;
}