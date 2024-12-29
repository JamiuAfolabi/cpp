#include <iostream>
#include <cstring>

#define MAX_STRING_LENGTH 50

class StringManager {
public:
    StringManager() : ptr(nullptr) {}

    // Copy a string into the buffer safely
    void copyString(const char* source) {
        size_t len = strlen(source);
        if (len + 1 <= MAX_STRING_LENGTH) {  // +1 for null terminator
            ptr = (char*)malloc(len + 1);
            if (ptr) {
                std::memcpy(ptr, source, len + 1);
            } else {
                std::cout << "Memory allocation failed." << std::endl;
            }
        } else {
            std::cout << "String too long." << std::endl;
        }
    }

    // Get the length of the stored string
    size_t getStringLength() const {
        if (ptr) {
            return strlen(ptr);
        } else {
            return 0;
        }
    }

    // Clear the string and free the allocated memory
    void clear() {
        free(ptr);
        ptr = nullptr;
    }

    // Print the stored string
    void printString() const {
        if (ptr) {
            std::cout << "Stored String: " << ptr << std::endl;
        } else {
            std::cout << "No string stored." << std::endl;
        }
    }

    ~StringManager() {
        clear();
    }

private:
    char* ptr;
};

int main() {
    StringManager strManager;

    strManager.copyString("Hello, Microcontroller!");
    strManager.printString();
    std::cout << "String Length: " << strManager.getStringLength() << std::endl;

    strManager.copyString("This string is much longer than the buffer size");
    
    strManager.clear();
    strManager.printString();

    return 0;
}
