#include <iostream>
#include <string>

int main() {
    std::string eventName;
    std::cout << "Enter the event name: ";
    std::getline(std::cin, eventName);

    std::string description;
    std::cout << "Enter the event description: ";
    std::getline(std::cin, description);

    // Process the strings as needed
    std::cout << "Event Name: " << eventName << std::endl;
    std::cout << "Event Description: " << description << std::endl;

    return 0;
}