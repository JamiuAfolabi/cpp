
#include <iostream>

// Constants for string buffer sizes
const size_t MAX_INPUT_LEN = 20;
const size_t MAX_RESULT_LEN = 40;

void example_function() {
    // Pre-allocate buffers for strings
    char input_buffer[MAX_INPUT_LEN] = {0};
    char result_buffer[MAX_RESULT_LEN] = {0};

    // Read input string from user (assuming you have a function to do this)
    read_user_input(input_buffer, MAX_INPUT_LEN);

    size_t input_len = strlen(input_buffer);

    // Perform string processing here
    // For example, concatenate "Hello, " with the input
    strcpy(result_buffer, "Hello, ");

    // Check if the concatenated string will fit in the buffer
    if (input_len + 7 <= MAX_RESULT_LEN) {
        strcat(result_buffer, input_buffer);
        // Output the result
        std::cout << "Result: " << result_buffer << std::endl;
    } else {
        std::cout << "Error: Input string too long." << std::endl;
    }
}

int main() {
    example_function();
    return 0;
}
