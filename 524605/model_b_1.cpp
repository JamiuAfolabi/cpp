#include <iostream>
#include <string>
#include <memory>
#include <exception>

class SensitiveDataException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Sensitive data manipulation failed!";
    }
};

class SecureString {
private:
    std::unique_ptr<char[]> data;
    size_t size;

public:
    SecureString() : data(nullptr), size(0) {}  // Empty constructor

    // Constructor from string literal
    SecureString(const char* str) {
        size = strlen(str) + 1;  // +1 for null terminator
        data = std::make_unique<char[]>(size);
        memcpy(data.get(), str, size);
    }

    // Constructor from std::string
    SecureString(const std::string& str) {
        size = str.length() + 1;
        data = std::make_unique<char[]>(size);
        memcpy(data.get(), str.c_str(), size);
    }

    // Copy constructor (disabled for security reasons)
    SecureString(const SecureString& other) = delete;

    // Move constructor
    SecureString(SecureString&& other) noexcept {
        data = std::move(other.data);
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }

    // Assignment operator (disabled for security reasons)
    SecureString& operator=(const SecureString& other) = delete;

    // Move assignment operator
    SecureString& operator=(SecureString&& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
        return *this;
    }

    ~SecureString() {
        // Override the destructor to securely zero out the memory
        if (data) {
            memset(data.get(), 0, size);
        }
    }

    // Clear the data and zero out the memory
    void clear() {
        if (data) {
            memset(data.get(), 0, size);
            size = 0;
        }
    }

    // Access the raw data pointer (disabled for security reasons)
    char* data() const = delete;

    // Get the size of the string
    size_t length() const {
        return size - 1;  // -1 to exclude the null terminator
    }

    // Concatenate another SecureString
    void append(const SecureString& other) {
        size_t newSize = size + other.size;
        auto newData = std::make_unique<char[]>(newSize);
        memcpy(newData.get(), data.get(), size);
        memcpy(newData.get() + size, other.data.get(), other.size);
        data = std::move(newData);
        size = newSize;
    }

    // Perform case-insensitive comparison (for equality)
    bool operator==(const SecureString& other) const {
        if (size != other.size) {
            return false;
        }

        return memcmp(data.get(), other.data.get(), size) == 0;
    }

    // Display the content of the string (for debugging only)
    void debugPrint() const {
        std::cout << "SecureString: ";
        for (size_t i = 0; i < size - 1; ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
    }
};

// Example usage:
int main() {
    try {
        SecureString pin("1234");
        pin.debugPrint();

        SecureString ssn("123-45-6789");
        ssn.debugPrint();

        SecureString tin("98-7654321");