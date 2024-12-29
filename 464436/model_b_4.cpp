#include <iostream>
#include <string>

int main() {
    std::string color;
    
    std::cout << "Enter a color (red, blue, green): ";
    std::cin >> color;

    std::transform(color.begin(), color.end(), color.begin(), ::tolower);

    switch (color) {
        case "red":
            std::cout << "You chose red. Red signifies passion and energy." << std::endl;
            break;
        case "blue":
            std::cout << "You chose blue. Blue signifies calm and trust." << std::endl;
            break;
        case "green":
            std::cout << "You chose green. Green signifies nature and growth." << std::endl;
            break;
        default:
            std::cout << "Unknown color. Please choose red, blue, or green." << std::endl;
            break;
    }

    return 0;
}