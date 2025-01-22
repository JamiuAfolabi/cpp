#include <iostream>
#include <cstring>
#include <stdexcept>

class StringHandler {
private:
    char *message;
    size_t length;

public:
    StringHandler() : message(nullptr), length(0) {
        // Empty string
    }

    StringHandler(const char *str) {
        if (str == nullptr) {
            throw std::runtime_error("Cannot create StringHandler from nullptr");
        }
        length = strlen(str);
        message = new char[length + 1];
        strcpy(message, str);
    }

    // Copy constructor
    StringHandler(const StringHandler& other) {
        length = other.length;
        if (length == 0) {
            message = nullptr;
        } else {
            message = new char[length + 1];
            strcpy(message, other.message);
        }
    }

    // Assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this == &other) {
            return *this;
        }

        delete[] message;
        length = other.length;
        if (length == 0) {
            message = nullptr;
        } else {
            message = new char[length + 1];
            strcpy(message, other.message);
        }
        return *this;
    }

    ~StringHandler() {
        delete[] message;
    }

    // Safe string copying
    void setMessage(const char *str) {
        if (str == nullptr) {
            throw std::runtime_error("Cannot set message to nullptr");
        }
        delete[] message;
        length = strlen(str);
        message = new char[length + 1];
        strcpy(message, str);
    }

    size_t getLength() const {
        return length;
    }

    const char* getMessage() const {
        return message;
    }

    // Concatenate two messages
    void concatenate(const StringHandler& other) {
        size_t newLength = length + other.length + 1;
        char *newMessage = new char[newLength];
        strcpy(newMessage, message);
        strcpy(newMessage + length, other.message);

        delete[] message;
        message = newMessage;
        length = newLength - 1;
    }

    void printMessage() const {
        std::cout << "Message: " << getMessage() << std::endl;
    }
};


int main() {
    try {
        {
            StringHandler str1("Hello");
            StringHandler str2(str1);

            str1.concatenate(StringHandler(" World!"));
            str1.printMessage();  // Output: Message: Hello World!
            str2.printMessage();  // Output: Message: Hello
        }

        StringHandler str3("This is a test");
        StringHandler str4 = str3;

        str3.concatenate(StringHandler());  // Concatenate with empty string
        str4.concatenate(StringHandler(" with another test"));

        str3.printMessage();  // Output: Message: This is a test
        str4.printMessage();  // Output: Message: This is a test with another test

        StringHandler str5("");
        str5.concatenate(StringHandler("Long string long string long string long string long string long string"));
        str5.printMessage();

        StringHandler str6;
        str6.concatenate(StringHandler("abc"));
        str6.printMessage();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
