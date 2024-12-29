#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <regex>
#include <cstring> // For strncpy and strlen

std::string parseInput(const std::string &input) {
    std::string parsed = input;

    // Define a regex pattern to remove unwanted punctuation (anything other than ?, ., and ,)
    std::regex pattern("[^a-zA-Z0-9 .,?]+");
    
    // Use regex_replace to remove unwanted punctuation
    parsed = std::regex_replace(parsed, pattern, "");

    // Remove extra spaces
    bool lastWasSpace = true; // To track if the last character was a space
    std::string finalParsed;
    for (char c : parsed) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                finalParsed += ' ';
            }
            lastWasSpace = true;
        } else {
            finalParsed += c;
            lastWasSpace = false;
        }
    }

    // Trim any trailing spaces
    if (!finalParsed.empty() && finalParsed.back() == ' ') {
        finalParsed.pop_back();
    }

    return finalParsed;
}

int main() {
    std::ofstream outFile("training_data.txt"); // Open a file to save the input
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1; // Exit if the file cannot be opened
    }

    char buffer[21] = {}; // Create a buffer of size 21 (20 characters + null terminator)
    const int maxBufferSize = 20; // To ensure we don't exceed the buffer size

    std::string userInput;
    int lineCount = 0; // To count the number of lines entered by the user

    std::cout << "Please enter up to 5 lines of text. Press Enter after each line." << std::endl;

    // Loop to get user input until 5 lines are entered
    while (lineCount < 5) {
        std::getline(std::cin, userInput); // Get a line of text from the user

        std::string parsedInput = parseInput(userInput); // Parse the input line

        int length = parsedInput.length(); // Get the length of the parsed line
        if (length > maxBufferSize) {
            parsedInput.resize(maxBufferSize); // Truncate if it exceeds the buffer size
        }

        // Copy the parsed input to the buffer, truncating if necessary
        strncpy(buffer, parsedInput.c_str(), maxBufferSize);
        buffer[maxBufferSize] = '\0';

        if (!parsedInput.empty()) { // Write only non-empty parsed lines to the file
            outFile << buffer << std::endl; // Save the line to the file
            lineCount++; // Increment the line count
        }
    }

    outFile.close(); // Close the file after writing
    std::cout << "Thank you! Your input has been saved to 'training_data.txt'." << std::endl;

    return 0; // Return success
}

