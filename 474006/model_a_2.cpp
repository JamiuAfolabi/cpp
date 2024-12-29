#include <iostream>

#define MAX_STRING_LENGTH 50

class StringManager {
public:
    // Constructor initializes an empty string
    StringManager() {
        clear();
    }

    // Method to copy a string into the buffer safely
    void copyString(const char* source) {
        // Manual copy to avoid strncpy, ensuring we stay within bounds
        size_t i = 0;
        while (source[i] != '\0' && i < MAX_STRING_LENGTH - 1) {
            buffer[i] = source[i];
            i++;
        }
        buffer[i] = '\0'; // Ensure null termination
    }

    // Get the length of the stored string
    size_t getStringLength() const {
        size_t length = 0;
        while (buffer[length] != '\0') {
            length++;
        }
        return length;
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
    strManager.copyString("Hello, Memory Efficient World!");
    strManager.printString();
    std::cout << "String Length: " << strManager.getStringLength() << std::endl;

    strManager.clear();
    strManager.printString();

    return 0;
}