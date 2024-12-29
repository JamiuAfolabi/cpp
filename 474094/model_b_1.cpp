#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // Standard filepath, you may modify this as needed
    string filePath = "training_data.txt";

    ofstream outputFile(filePath);

    if (!outputFile.is_open()) {
        cout << "Failed to open the file! Please check the filename and path." << endl;
        return 1;
    }
    cout << "Enter text for training data (5 lines max):\n";
    string input;
    int count = 0;

    while (getline(cin, input) && count < 5) {
        if (!input.empty()) {
            // Write the input line to the file with a newline character added
            outputFile << input << endl;
            count++;
        }
        if (count == 5) {
            cout << "\n Maximum input lines reached. Training data saved." << endl;
        }
    }

    outputFile.close();

    return 0;
}