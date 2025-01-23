
#include <iostream>
#include <cstring>

const int MAX_HEADLINES = 5;
const int MAX_LENGTH = 101;

int main() {
    char headlines[MAX_HEADLINES][MAX_LENGTH];

    for (int i = 0; i < MAX_HEADLINES; ++i) {
        std::cout << "Enter headline " << i + 1 << " (max " << MAX_LENGTH - 1 << " characters): ";
        std::cin.ignore();
        std::getline(std::cin, headlines[i]);

        if (strlen(headlines[i]) >= MAX_LENGTH) {
            std::cout << "Error: Headline length must be less than or equal to " << MAX_LENGTH - 1 << " characters." << std::endl;
            return 1;
        }
    }

    std::cout << "\nHeadlines stored in the array:\n";
    for (int i = 0; i < MAX_HEADLINES; ++i) {
        std::cout << "Headline " << i + 1 << ": " << headlines[i] << std::endl;
    }

    return 0;
}
  