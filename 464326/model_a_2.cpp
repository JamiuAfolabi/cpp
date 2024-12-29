#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Define constants for error messages and default filename
#define FILE_NOT_FOUND_MSG "Error: Could not open the file: "
#define INCORRECT_FORMAT_MSG "Error: Incorrect format in line: "
#define NO_VALID_DATA_MSG "No valid temperature data found."
#define DAILY_TEMPERATURES_MSG "Daily Temperatures:"
#define DEFAULT_FILENAME "temperature_data.txt" // Example default filename

void readTemperatureData(const std::string& filename) {
    std::ifstream file(filename);
    
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << FILE_NOT_FOUND_MSG << filename << std::endl;
        return;
    }
    
    std::string line;
    std::vector<double> temperatures;
    
    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        double temperature;
        
        // Attempt to parse the temperature from the line
        if (!(ss >> temperature)) {
            std::cerr << INCORRECT_FORMAT_MSG << line << std::endl;
            continue; // Skip to the next line
        }

        // Store the valid temperature
        temperatures.push_back(temperature);
    }

    // Close the file
    file.close();

    // Display the read temperatures
    if (!temperatures.empty()) {
        std::cout << DAILY_TEMPERATURES_MSG << std::endl;
        for (const auto& temp : temperatures) {
            std::cout << temp << std::endl;
        }
    } else {
        std::cout << NO_VALID_DATA_MSG << std::endl;
    }
}

int main() {
    std::string filename;

    std::cout << "Enter the name of the temperature data file (or press ENTER for default): ";
    std::getline(std::cin, filename);
    
    // Use default filename if user does not provide one
    if (filename.empty()) {
        filename = DEFAULT_FILENAME;
    }

    readTemperatureData(filename);

    return 0;
}