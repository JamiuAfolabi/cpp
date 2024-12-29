#include <iostream>
#include <unordered_map>

enum class CandyColor {
    RED,
    WHITE,
    BLUE,
    UNKNOWN
};

typedef void (*CandyAction)();

void sendCandyToPackaging() {
    std::cout << "Candy directed to packaging section." << std::endl;
}

void sendCandyToInspection() {
    std::cout << "Candy directed to inspection section." << std::endl;
}

void sendCandyToFurtherProcessing() {
    std::cout << "Candy directed to further processing section." << std::endl;
}

std::unordered_map<CandyColor, CandyAction> actionLUT = {
    { CandyColor::RED, &sendCandyToPackaging },
    { CandyColor::WHITE, &sendCandyToInspection },
    { CandyColor::BLUE, &sendCandyToFurtherProcessing }
};

void sortCandy(CandyColor candyColor) {
    auto action = actionLUT.find(candyColor);
    if (action != actionLUT.end()) {
        action->second();
    } else {
        std::cout << "Unknown candy color detected." << std::endl;
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