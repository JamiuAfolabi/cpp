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
    printf("Routing candy to packaging section...\n");
}

void sendCandyToInspection() {
    printf("Routing candy to inspection section...\n");
}

void sendCandyToFurtherProcessing() {
    printf("Routing candy to further processing section...\n");
}

std::unordered_map<CandyColor, CandyAction> actionLUT = {
    { CandyColor::RED, &sendCandyToPackaging },
    { CandyColor::WHITE, &sendCandyToInspection },
    { CandyColor::BLUE, &sendCandyToFurtherProcessing }
};

const char* candyColorToString(CandyColor color) {
    switch (color) {
        case CandyColor::RED: return "RED";
        case CandyColor::WHITE: return "WHITE";
        case CandyColor::BLUE: return "BLUE";
        default: return "UNKNOWN";
    }
}

void sortCandy(CandyColor candyColor) {
    printf("Processing candy of color: %s\n", candyColorToString(candyColor));
    
    auto action = actionLUT.find(candyColor);
    if (action != actionLUT.end()) {
        printf("Found action for %s candy. Executing action...\n", candyColorToString(candyColor));
        action->second();
    } else {
        printf("No action found for %s candy. Unknown color.\n", candyColorToString(candyColor));
    }
}

int main() {
    CandyColor candyColor1 = CandyColor::RED;
    CandyColor candyColor2 = CandyColor::WHITE;
    CandyColor candyColor3 = CandyColor::BLUE;
    CandyColor candyColor4 = CandyColor::UNKNOWN;

    printf("Start sorting candies...\n\n");
    sortCandy(candyColor1); // Output: Routing candy to packaging section.
    sortCandy(candyColor2); // Output: Routing candy to inspection section.
    sortCandy(candyColor3); // Output: Routing candy to further processing section.
    sortCandy(candyColor4); // Output: No action found for unknown candy color.

    printf("\nCandy sorting complete.\n");
    return 0;
}