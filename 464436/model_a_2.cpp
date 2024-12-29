#include <iostream>

enum class Color { Red, Green, Blue };

int main() {
    Color color = Color::Green;

    switch (color) {
        case Color::Red:
            std::cout << "Color is Red" << std::endl;
            break;
        case Color::Green:
            std::cout << "Color is Green" << std::endl;
            break;
        case Color::Blue:
            std::cout << "Color is Blue" << std::endl;
            break;
        default:
            std::cout << "Unknown color" << std::endl;
            break;
    }

    return 0;
}