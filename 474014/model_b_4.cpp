
#include <iostream>
#include <string>
#include <vector>

// Preprocessor constant to choose between fixed-size and dynamic buffer
#define USE_FIXED_SIZE_BUFFER 1  // Set to 1 for fixed-size, 0 for dynamic

class CustomStringBuffer {
public:
    explicit CustomStringBuffer(size_t size) : maxSize_(size) {
        #if USE_FIXED_SIZE_BUFFER
            buffer_ = new char[size_ + 1];
            buffer_[0] = '\0';
        #else
            buffer_.resize(size_ + 1, '\0');
        #endif
    }

    ~CustomStringBuffer() {
        #if USE_FIXED_SIZE_BUFFER
            delete[] buffer_;
        #endif
    }
    // Clear the buffer
    void clear() {
        buffer_[0] = '\0';
    }

    // Check if the string buffer is empty
    bool empty() const {
        return buffer_[0] == '\0';
    }

    // Check if the buffer is full
    bool full() const {
        return strlen(buffer_.data()) == maxSize_;
    }
    
    // Set the string in the buffer (safety: Truncate string if it's longer than the buffer size)
    void set_string(const char* str) {
        if (!str) {
            clear();
            return;
        }
        
        size_t str_len = strlen(str);
        if (str_len >= maxSize_) {
            std::cerr << "WARNING: String too long for the buffer, truncating.\n";
            str_len = maxSize_;
        }
        std::strncpy(buffer_.data(), str, str_len);
        buffer_[str_len] = '\0'; // Ensure null-termination
    }
    
    // Get the string from the buffer
    const char* get_string() const {
        return buffer_.data();
    }

    // Return the length of the string in the buffer
    size_t length() const {
        return strlen(buffer_.data());
    }


   // Rest of the methods remain the same...

private:
    std::vector<char> buffer_;
    size_t maxSize_; // Maximum size of the buffer

};

int main() {
    const size_t BUFFER_SIZE = 16;
    CustomStringBuffer rfidBuffer(BUFFER_SIZE);
    
    // Rest of the code remains the same...
    const char* inputString1 = "Hello, RFID!";
    rfidBuffer.set_string(inputString1);
    std::cout << "Written to RFID: " << rfidBuffer.get_string() << " (length: " << rfidBuffer.length() << ")\n";
    
    if (rfidBuffer.full()) {
        std::cout << "Buffer is full." << std::endl;
    }

    const char* inputString2 = "This is a longer string.";
    rfidBuffer.set_string(inputString2);
    std::cout << "Written to RFID: " << rfidBuffer.get_string() << " (length: " << rfidBuffer.length() << ")\n"; 
    

    return 0;
}