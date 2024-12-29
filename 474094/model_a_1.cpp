#include <iostream>
#include <fstream>
#include <string>

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
        outFile << userInput << std::endl; // Save the line to the file
        lineCount++; // Increment the line count
    }

    outFile.close(); // Close the file after writing
    std::cout << "Thank you! Your input has been saved to 'training_data.txt'." << std::endl;
    
    return 0; // Return success
}