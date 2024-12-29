#include <stdio.h>
#include <unordered_map>

enum class CandyColor {
    RED,
    WHITE,
    BLUE,
    UNKNOWN
};

typedef void (*CandyAction)();

void sendCandyToPackaging() {
    printf("Candy directed to packaging section.\n");
}

void sendCandyToInspection() {
    printf("Candy directed to inspection section.\n");
}

void sendCandyToFurtherProcessing() {
    printf("Candy directed to further processing section.\n");
}

std::unordered_map<CandyColor, CandyAction> actionLUT = {
    { CandyColor::RED, &sendCandyToPackaging },
    { CandyColor::WHITE, &sendCandyToInspection },
    { CandyColor::BLUE, &sendCandyToFurtherProcessing }
};

void sortCandy(CandyColor candyColor) {
    const char* colorName;
    switch (candyColor) {
        case CandyColor::RED:
            colorName = "Red";
            break;
        case CandyColor::WHITE:
            colorName = "White";
            break;
        case CandyColor::BLUE:
            colorName = "Blue";
            break;
        default:
            colorName = "Unknown";
            break;
    }
    
    printf("Processing a %s candy...\n", colorName);

    auto action = actionLUT.find(candyColor);
    if (action != actionLUT.end()) {
        printf("Action found for %s candy.\n", colorName);
        action->second();
    } else {
        printf("Unknown candy color: %s detected. Skipping.\n", colorName);
    }
}

int main() {
    CandyColor candyColor1 = CandyColor::RED;
    CandyColor candyColor2 = CandyColor::WHITE;
    CandyColor candyColor3 = CandyColor::BLUE;
    CandyColor candyColor4 = CandyColor::UNKNOWN;    
    CandyColor candyColor5 = (CandyColor)10; // Invalid color

    sortCandy(candyColor1);
    sortCandy(candyColor2);
    sortCandy(candyColor3);
    sortCandy(candyColor4);
    sortCandy(candyColor5);

    return 0;
}