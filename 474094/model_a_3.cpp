#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <regex>
#include <cstring>

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

    std::string userInput;
    int lineCount = 0; // To count the number of lines entered by the user

    std::cout << "Please enter up to 5 lines of text. Press Enter after each line." << std::endl;

    // Loop to get user input until 5 lines are entered
    while (lineCount < 5) {
        std::getline(std::cin, userInput); // Get a line of text from the user

        std::string parsedInput = parseInput(userInput); // Parse the input line

        if (!parsedInput.empty()) { // Write only non-empty parsed lines to the file
            // Create a buffer for the parsed input
            char buffer[21]; // +1 for the null terminator

            // Truncate the parsedInput if it exceeds 20 characters using strncpy
            strncpy(buffer, parsedInput.c_str(), 20);
            buffer[20] = '\0'; // Ensure the string is null-terminated

            // Output to the file
            outFile << buffer << std::endl; // Save the line to the file
            lineCount++; // Increment the line count
        }
    }

    outFile.close(); // Close the file after writing
    std::cout << "Thank you! Your input has been saved to 'training_data.txt'." << std::endl;

    return 0; // Return success
}