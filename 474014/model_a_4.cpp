#include <iostream>
#include <vector>
#include <cstring>

// Uncomment the following line to use a dynamic buffer instead of a fixed size
#define USE_DYNAMIC_BUFFER

const size_t FIXED_BUFFER_SIZE = 16; // Set a fixed buffer size

class CustomStringBuffer {
public:
    CustomStringBuffer() {
#ifdef USE_DYNAMIC_BUFFER
        // Dynamic buffer
        buffer_.resize(1, '\0'); // Start with just the null terminator
#else
        // Fixed size buffer
        buffer_.resize(FIXED_BUFFER_SIZE + 1, '\0'); // Include space for the null terminator
#endif
    }
    
    void clear() {
        buffer_[0] = '\0'; // Reset buffer to empty string
    }
    
    bool empty() const {
        return buffer_[0] == '\0';
    }
    
    void set_string(const char* str) {
        if (!str) {
            clear();
            return;
        }

#ifdef USE_DYNAMIC_BUFFER
        // Dynamic buffer: Grow dynamically to accommodate the string
        buffer_.assign(str, str + strlen(str)); // Copy the string
        buffer_.push_back('\0'); // Add null terminator
#else
        // Fixed buffer: Truncate if necessary
        size_t str_len = strlen(str);
        if (str_len >= FIXED_BUFFER_SIZE) {
            std::cerr << "WARNING: String too long for the fixed-size buffer, truncating.\n";
            str_len = FIXED_BUFFER_SIZE;
        }
        std::strncpy(buffer_.data(), str, str_len);
        buffer_[str_len] = '\0'; // Ensure null terminator
#endif
    }

    const char* get_string() const {
        return buffer_.data();
    }

    size_t length() const {
        return strlen(buffer_.data());
    }

private:
    std::vector<char> buffer_;
};

int main() {
    CustomStringBuffer stringBuffer;
    
    const char* inputString = "Hello, World!";
    stringBuffer.set_string(inputString);
    std::cout << "Stored String: " << stringBuffer.get_string() << " (length: " << stringBuffer.length() << ")\n";

    const char* longString = "This string is definitely longer than the fixed buffer size!";
    stringBuffer.set_string(longString);
    std::cout << "Stored String: " << stringBuffer.get_string() << " (length: " << stringBuffer.length() << ")\n";

    return 0;
}