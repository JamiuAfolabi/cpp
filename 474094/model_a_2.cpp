#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

std::string parseInput(const std::string &input) {
    std::string parsed;
    bool lastWasSpace = true; // To track if the last character was a space

    for (char c : input) {
        // Check if the character is printable or one of the allowed symbols
        if (std::isprint(c) || c == '?' || c == ',' || c == '.') {
            if (std::isspace(c)) {
                // Add a single space if the last character was not a space
                if (!lastWasSpace) {
                    parsed += ' ';
                }
                lastWasSpace = true;
            } else {
                parsed += c; // Add the printable character or allowed symbol
                lastWasSpace = false; // Reset the space flag
            }
        }
    }

    // Trim any trailing spaces
    if (!parsed.empty() && parsed.back() == ' ') {
        parsed.pop_back();
    }

    return parsed;
}

int main() {
    std::ofstream outFile("training_data.txt"); // Open a file to save the input
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1; // Exit if the file cannot be opened
    }

    std::string userInput;
    int lineCount = 0; // To count the number of lines entered by the user

    std::cout << "Please enter up to 5 lines of text. Press Enter after each line." << std::endl;

    // Loop to get user input until 5 lines are entered
    while (lineCount < 5) {
        std::getline(std::cin, userInput); // Get a line of text from the user

        std::string parsedInput = parseInput(userInput); // Parse the input line

        if (!parsedInput.empty()) { // Write only non-empty parsed lines to the file
            outFile << parsedInput << std::endl; // Save the line to the file
            lineCount++; // Increment the line count
        }
    }

    outFile.close(); // Close the file after writing
    std::cout << "Thank you! Your input has been saved to 'training_data.txt'." << std::endl;

    return 0; // Return success
}