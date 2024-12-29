#include <iostream>
#include <cstring>

class StringBuffer {
private:
    static const size_t BUFFER_SIZE = 256; // Fixed size for the buffer
    char buffer[BUFFER_SIZE];               // Internal buffer
    size_t currentLength;                    // Current length of stored string

public:
    StringBuffer() : currentLength(0) {
        buffer[0] = '\0'; // Initialize buffer to empty string
    }

    // Function to add a string to the buffer
    bool addString(const char* str) {
        size_t len = strlen(str);
        if (len + currentLength >= BUFFER_SIZE) {
            std::cerr << "Error: Buffer overflow prevented. Cannot add string." << std::endl;
            return false; // Not enough space
        }
        
        strcpy(buffer + currentLength, str); // Copy string to buffer
        currentLength += len;                 // Update current length
        return true;                          // Successfully added string
    }

    // Function to retrieve the stored string
    const char* getString() const {
        return buffer; // Return the buffer contents
    }

    // Function to clear the buffer
    void clear() {
        buffer[0] = '\0';       // Clear buffer
        currentLength = 0;      // Reset current length
    }

    // Function to get the current length of the stored string
    size_t length() const {
        return currentLength;    // Return the length of the stored string
    }
};

int main() {
    StringBuffer strBuffer;

    // Example usage
    strBuffer.addString("Hello, ");
    strBuffer.addString("world!");
    
    std::cout << "Stored String: " << strBuffer.getString() << std::endl;
    std::cout << "String Length: " << strBuffer.length() << std::endl;

    // Attempt to add a long string
    if (!strBuffer.addString(" This string is too long to fit without causing an overflow.")) {
        std::cout << "Failed to add exceeding string." << std::endl;
    }

    strBuffer.clear();
    std::cout << "Buffer cleared. Current Length: " << strBuffer.length() << std::endl;

    return 0;
}