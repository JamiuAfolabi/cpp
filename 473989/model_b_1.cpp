#include <iostream>

enum class CandyColor {
    RED,
    WHITE,
    BLUE,
    UNKNOWN
};

void sendCandyToPackaging() {
    std::cout << "Candy directed to packaging section." << std::endl;
}

void sendCandyToInspection() {
    std::cout << "Candy directed to inspection section." << std::endl;
}

void sendCandyToFurtherProcessing() {
    std::cout << "Candy directed to further processing section." << std::endl;
}

void sortCandy(CandyColor candyColor) {
    switch (candyColor) {
        case CandyColor::RED:
            sendCandyToPackaging();
            break;
        case CandyColor::WHITE:
            sendCandyToInspection();
            break;
        case CandyColor::BLUE:
            sendCandyToFurtherProcessing();
            break;
        default:
            std::cout << "Unknown candy color detected." << std::endl;
            break;
    }
}

int main() {
    CandyColor candyColor1 = CandyColor::RED;
    CandyColor candyColor2 = CandyColor::WHITE;
    CandyColor candyColor3 = CandyColor::BLUE;
    CandyColor candyColor4 = CandyColor::UNKNOWN;

    sortCandy(candyColor1); // Output: Candy directed to packaging section.
    sortCandy(candyColor2); // Output: Candy directed to inspection section.
    sortCandy(candyColor3); // Output: Candy directed to further processing section.
    sortCandy(candyColor4); // Output: Unknown candy color detected.

    return 0;
}