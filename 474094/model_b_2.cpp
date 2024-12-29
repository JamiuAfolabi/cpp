#include <iostream>
#include <fstream>
#include <string>
#include <cctype>   // For isprint and isspace functions
#include <algorithm> // For erase function

std::string parseInputLine(const std::string& input) {
    std::string parsedLine = input;
    
    // Remove unprintable characters and extra whitespaces
    parsedLine.erase(std::remove_if(parsedLine.begin(), parsedLine.end(), [](char ch) {
        return !std::isprint(ch) || std::isspace(ch) && ch != ' ';
    }), parsedLine.end());

    // Remove any remaining spaces
    parsedLine.erase(std::remove(parsedLine.begin(), parsedLine.end(), ' '), parsedLine.end());

    // Keep only the allowed symbols: ?, ,, .
    parsedLine.erase(std::remove_if(parsedLine.begin(), parsedLine.end(), [](char ch) {
        return ch != '?' && ch != ',' && ch != '.';
    }), parsedLine.end());

    return parsedLine;
}

int main() {
    std::ofstream outFile("training_data.txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    std::string userInput;
    int lineCount = 0;

    std::cout << "Please enter up to 5 lines of text. Press Enter after each line." << std::endl;

    while (lineCount < 5) {
        std::getline(std::cin, userInput);
        userInput = parseInputLine(userInput);  // Parse the input before writing
        outFile << userInput << std::endl;
        lineCount++;
    }

    outFile.close();
    std::cout << "Thank you! Your input has been saved to 'training_data.txt'." << std::endl;

    return 0;
}