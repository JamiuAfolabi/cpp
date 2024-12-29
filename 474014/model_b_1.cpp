
#include <iostream>
#include <string>

class CustomStringBuffer {
public:
    explicit CustomStringBuffer(size_t size) : size_(size) {
        // Include space for null terminator
        buffer_ = new char[size_ + 1];
        buffer_[0] = '\0';
    }

    ~CustomStringBuffer() {
        delete[] buffer_;
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
        return strlen(buffer_) == size_;
    }
    // Set the string in the buffer (safety: Truncate string if it's longer than the buffer size)
    void set_string(const char* str) {
        if (!str) {
            clear();
            return;
        }
        
        size_t str_len = strlen(str);
        if (str_len >= size_) {
            std::cerr << "WARNING: String too long for the buffer, truncating.\n";
            str_len = size_;
        }
        strcpy(buffer_, str);
        buffer_[str_len] = '\0';
    }
    
    // Get the string from the buffer
    const char* get_string() const {
        return buffer_;
    }

    // Return the length of the string in the buffer
    size_t length() const {
        return strlen(buffer_);
    }

   
private:
    char* buffer_;
    size_t size_;
};

int main() {
    CustomStringBuffer rfidBuffer(16); // 16 characters including the null terminator

    const char* inputString1 = "Hello, world!";
    rfidBuffer.set_string(inputString1);
    std::cout << "Written to RFID: " << rfidBuffer.get_string() << " (length: " << rfidBuffer.length() << ")\n";
    
    if (rfidBuffer.full()) {
        std::cout << "Buffer is full." << std::endl;
    }

    const char* inputString2 = "This is longer than the buffer";
    rfidBuffer.set_string(inputString2);
    std::cout << "Written to RFID: " << rfidBuffer.get_string() << " (length: " << rfidBuffer.length() << ")\n"; 
    
    return 0;
}
