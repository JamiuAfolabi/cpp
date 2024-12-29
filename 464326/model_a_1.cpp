#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void readTemperatureData(const std::string& filename) {
    std::ifstream file(filename);
    
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filename << std::endl;
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
            std::cerr << "Error: Incorrect format in line: " << line << std::endl;
            continue; // Skip to the next line
        }

        // Store the valid temperature
        temperatures.push_back(temperature);
    }

    // Close the file
    file.close();

    // Display the read temperatures
    if (!temperatures.empty()) {
        std::cout << "Daily Temperatures:" << std::endl;
        for (const auto& temp : temperatures) {
            std::cout << temp << std::endl;
        }
    } else {
        std::cout << "No valid temperature data found." << std::endl;
    }
}

int main() {
    std::string filename;

    std::cout << "Enter the name of the temperature data file: ";
    std::cin >> filename;

    readTemperatureData(filename);

    return 0;
}