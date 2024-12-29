#include <iostream>
#include <string>

enum Color { RED, BLUE, GREEN, UNKNOWN };

Color getColor(const std::string& color) {
    if (color == "red") return RED;
    if (color == "blue") return BLUE;
    if (color == "green") return GREEN;
    return UNKNOWN;
}

int main() {
    std::string colorInput;

    std::cout << "Enter a color (red, blue, green): ";
    std::cin >> colorInput;

    Color color = getColor(colorInput);

    switch (color) {
        case RED:
            std::cout << "You chose red. Red signifies passion and energy." << std::endl;
            break;
        case BLUE:
            std::cout << "You chose blue. Blue signifies calm and trust." << std::endl;
            break;
        case GREEN:
            std::cout << "You chose green. Green signifies nature and growth." << std::endl;
            break;
        case UNKNOWN:
        default:
            std::cout << "Unknown color. Please choose red, blue, or green." << std::endl;
            break;
    }

    return 0;
}