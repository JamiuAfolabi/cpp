#include <iostream>
#include <cstring>

#define MAX_STRING_LENGTH 50

class StringManager {
public:
    // Constructor initializes an empty string
    StringManager() {
        clear();
    }

    // Copy a string into the buffer safely
    void copyString(const char* source) {
        // Use strncpy to prevent buffer overflow
        strncpy(buffer, source, MAX_STRING_LENGTH - 1);
        buffer[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null termination
    }

    // Get the length of the stored string
    size_t getStringLength() const {
        return strlen(buffer);
    }

    // Clear the string
    void clear() {
        buffer[0] = '\0'; // Set the first character to null to clear the string
    }

    // Print the stored string
    void printString() const {
        std::cout << "Stored String: " << buffer << std::endl;
    }

private:
    char buffer[MAX_STRING_LENGTH]; // Fixed-size buffer
};

int main() {
    StringManager strManager;

    // Example usage
    strManager.copyString("Hello, Microcontroller!");
    strManager.printString();
    std::cout << "String Length: " << strManager.getStringLength() << std::endl;

    strManager.clear();
    strManager.printString();

    return 0;
}