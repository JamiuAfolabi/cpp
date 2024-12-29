#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

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
    std::ofstream outFile("training_data.txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    int lineCount = 0;
    std::cout << "Please enter up to 5 lines of text:\n";

    while (lineCount < 5) {
        std::string userInput;
        std::getline(std::cin, userInput);

        std::string parsedInput = parseInput(userInput);

        if (!parsedInput.empty()) {
            // using std::stringstream and std::limiter
            std::stringstream ss;
            std::copy_n(parsedInput.begin(), 20, std::ostreambuf_iterator<char>(ss));
            outFile << ss.str() << '\n';
            lineCount++;
        }
    }

    outFile.close();
    std::cout << "Input saved to training_data.txt" << std::endl;

    return 0;
}