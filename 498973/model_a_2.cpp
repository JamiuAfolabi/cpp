#include <iostream>
#include <string>
#include <stdexcept>

const size_t MAX_BUFFER_SIZE = 20;

void handleEvent(const std::string& eventName) {
    // Process the event
    std::cout << "Handling event: " << eventName << std::endl;
}

int main() {
    std::string eventInput;

    std::cout << "Enter the event name (max 20 characters): ";
    // Read the input
    std::getline(std::cin, eventInput);

    // Check the length of the input and handle appropriately
    if (eventInput.length() > MAX_BUFFER_SIZE) {
        std::cout << "Error: Input exceeds " << MAX_BUFFER_SIZE << " characters." << std::endl;
        return 1;  // Exit with an error code
    }

    handleEvent(eventInput);

    return 0;
}