#include <iostream>
#include <string>

int main() {
    const size_t MAX_BUFFER_SIZE = 20; // Your fixed-size buffer
    std::string fixedBuffer(MAX_BUFFER_SIZE, ' '); // Fixed size buffer initialization 
                                                   //And initialized to space
    fixedBuffer.reserve(MAX_BUFFER_SIZE);  // Reserve the fixed size memory
    
    std::cout << "Enter a string with at most " << MAX_BUFFER_SIZE - 1 << " characters: ";
    std::getline(std::cin, fixedBuffer, '\n');
    
    // Check if the input exceeded the buffer size
    if (fixedBuffer.length() > 0 && fixedBuffer[fixedBuffer.length() - 1] == '\n') {
        fixedBuffer.pop_back();  // Remove the newline character read by getline
    } else {
        std::cout << "\nInput exceeded the buffer size." << std::endl;
        fixedBuffer = "";  // Clear the buffer or handle the error as needed
    }

    std::cout << "Stored string in the fixed buffer: " << fixedBuffer << std::endl;

    return 0;
}