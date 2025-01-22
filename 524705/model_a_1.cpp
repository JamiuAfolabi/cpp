#include <iostream>
#include <cstring>
#include <stdexcept>

class StringHandler {
private:
    char* message;

public:
    // Default constructor
    StringHandler() : message(nullptr) {}

    // Parameterized constructor
    StringHandler(const char* msg) {
        if (msg) {
            // Allocate memory and copy the message
            message = new char[strlen(msg) + 1];
            strcpy(message, msg);
        } else {
            message = nullptr;
        }
    }

    // Copy constructor
    StringHandler(const StringHandler& other) {
        if (other.message) {
            message = new char[strlen(other.message) + 1];
            strcpy(message, other.message);
        } else {
            message = nullptr;
        }
    }

    // Assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this != &other) {
            // Clean up existing message
            delete[] message;

            if (other.message) {
                // Allocate new memory and copy the message
                message = new char[strlen(other.message) + 1];
                strcpy(message, other.message);
            } else {
                message = nullptr;
            }
        }
        return *this;
    }

    // Destructor
    ~StringHandler() {
        delete[] message;
    }

    // Function for safe string copying
    void copyMessage(const char* msg) {
        if (msg) {
            delete[] message; // Clean up existing message
            message = new char[strlen(msg) + 1];
            strcpy(message, msg);
        } else {
            throw std::invalid_argument("Null pointer provided for message");
        }
    }

    // Function for calculating length of the message
    size_t length() const {
        return message ? strlen(message) : 0;
    }

    // Function for concatenating messages
    void concat(const char* msg) {
        if (msg) {
            size_t newLength = length() + strlen(msg);
            char* newMessage = new char[newLength + 1];

            if (message) {
                strcpy(newMessage, message); // Copy existing message
            }
            strcat(newMessage, msg); // Concatenate new message

            delete[] message; // Clean up old message
            message = newMessage; // Assign new message
        } else {
            throw std::invalid_argument("Null pointer provided for message");
        }
    }

    // Function to get the message (for demonstration)
    const char* getMessage() const {
        return message ? message : "";
    }
};

int main() {
    try {
        StringHandler sh1("Hello");
        StringHandler sh2;
        
        // Testing copy constructor
        sh2 = sh1; // Using assignment operator
        std::cout << "Message in sh2: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

        // Testing concatenation
        sh2.concat(" World!");
        std::cout << "After concatenation: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

        // Testing safe copying
        sh2.copyMessage("New Message");
        std::cout << "After copying: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}