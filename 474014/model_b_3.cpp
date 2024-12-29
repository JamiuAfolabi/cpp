#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class CustomStringBuffer {
public:
    explicit CustomStringBuffer(size_t size) : size_(size), data_(size + 1) {
        // Include space for null terminator
        data_[0] = '\0';
    }

    void set_string(const std::string& str) {
        if (str.size() >= size_) {
            throw std::length_error("String is too long for the buffer.");
        }
        str.copy(data_, str.size());
        data_[str.size()] = '\0';
    }

    const char* get_string() const {
        return data_.data();
    }

    size_t length() const {
        return strlen(data_.data());
    }

    bool empty() const {
        return data_[0] == '\0';
    }

private:
    size_t size_;
    std::vector<char> data_;
};

int main() {
    CustomStringBuffer rfidBuffer(16); // 16 characters including the null terminator

    std::string inputString1 = "Hello, world!";
    try {
        rfidBuffer.set_string(inputString1);
        std::cout << "Written to RFID: " << rfidBuffer.get_string() << " (length: " << rfidBuffer.length() << ")\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::string inputString2 = "This is a very long string that won't fit in the buffer";
    try {
        rfidBuffer.set_string(inputString2);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}