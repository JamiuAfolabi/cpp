#include <iostream>
#include <map>
#include <functional>

enum class CandyColor {
    RED,
    WHITE,
    BLUE,
    UNKNOWN
};

// Action functions
void sendCandyToPackaging() {
    std::cout << "Candy directed to packaging section." << std::endl;
}

void sendCandyToInspection() {
    std::cout << "Candy directed to inspection section." << std::endl;
}

void sendCandyToFurtherProcessing() {
    std::cout << "Candy directed to further processing section." << std::endl;
}

// Main Control Class
class CandySorter {
public:
    CandySorter() {
        // Initialize LUT mapping
        actionMap[CandyColor::RED] = sendCandyToPackaging;
        actionMap[CandyColor::WHITE] = sendCandyToInspection;
        actionMap[CandyColor::BLUE] = sendCandyToFurtherProcessing;
        actionMap[CandyColor::UNKNOWN] = []() {
            std::cout << "Unknown candy color detected." << std::endl;
        };
    }

    void sortCandy(CandyColor candyColor) {
        // Find the action and execute it
        if (actionMap.find(candyColor) != actionMap.end()) {
            actionMap[candyColor]();
        } else {
            actionMap[CandyColor::UNKNOWN](); // Handle unknown colors
        }
    }

private:
    std::map<CandyColor, std::function<void()>> actionMap;
};

int main() {
    CandySorter sorter;
    
    // Example candies
    sorter.sortCandy(CandyColor::RED);     // Output: Candy directed to packaging section.
    sorter.sortCandy(CandyColor::WHITE);   // Output: Candy directed to inspection section.
    sorter.sortCandy(CandyColor::BLUE);    // Output: Candy directed to further processing section.
    sorter.sortCandy(CandyColor::UNKNOWN);  // Output: Unknown candy color detected.

    return 0;
}