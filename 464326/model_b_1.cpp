#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Function to read temperature data from the file
vector<float> readTemperatureData(const string& filename) {
    vector<float> temperatures;
    ifstream file(filename);

    if (!file.is_open()) {
        // File not found
        cerr << "Error: File not found: " << filename << endl;
        return temperatures;
    }

    string line;
    while (getline(file, line)) {
        stringstream iss(line);
        float temperature;

        if (!(iss >> temperature)) {
            // Incorrect format: Unable to convert to float
            cerr << "Error: Incorrect format at line: " << line << endl;
            return temperatures;
        }

        // Check for negative temperatures, if that's not expected
        // if (temperature < 0.0f) {
        //     cerr << "Error: Invalid temperature: " << temperature << " (expected non-negative)" << endl;
        //     return temperatures;
        // }

        temperatures.push_back(temperature);
    }

    file.close();

    // Successful read, return the temperatures
    return temperatures;
}

int main() {
    string filename;
    cout << "Enter the text file name containing temperature data: ";
    getline(cin, filename);

    vector<float> temperatures = readTemperatureData(filename);

    if (temperatures.empty()) {
        // Error occurred during reading, exit the program
        cerr << "Failed to read temperature data. Exiting..." << endl;
        return 1;
    }

    // Display the read temperatures
    cout << "Read the following temperatures:" << endl;
    for (const float& temp : temperatures) {
        cout << temp << " ";
    }
    cout << endl;

    return 0;
}