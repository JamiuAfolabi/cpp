#include <iostream>
#include <string>

void handleEvent(const std::string& eventName) {
    // Process the event
    std::cout << "Handling event: " << eventName << std::endl;
}

int main() {
    std::string eventInput;

    std::cout << "Enter the event name: ";
    std::getline(std::cin, eventInput);  // Safely read a line of input

    handleEvent(eventInput);

    return 0;
}