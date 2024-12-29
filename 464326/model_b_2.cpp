#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Error message constants
#define ERR_FILE_NOT_FOUND "Error: Could not open the file: %s\n"
#define ERR_INVALID_FORMAT "Error: Incorrect format in line: %s\n"
#define ERR_NO_VALID_DATA  "No valid temperature data found."

// Example using hard-coded filename constant, can be uncommented to use default
#define DEFAULT_TEMPERATURE_FILE "temperature_data.txt" 


void readTemperatureData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << std::printf(ERR_FILE_NOT_FOUND, filename.c_str());
        return;
    }

    std::string line;
    std::vector<double> temperatures;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        double temperature;
        
        if (!(ss >> temperature)) {
            std::cerr << std::printf(ERR_INVALID_FORMAT, line.c_str());
            continue;
        }
        temperatures.push_back(temperature);
    }
    
    file.close();
    
    if (!temperatures.empty()) {
        std::cout << "Daily Temperatures:" << std::endl;
        for (const auto& temp : temperatures) {
            std::cout << temp << std::endl;
        }
    } 
    else {
        std::cout << ERR_NO_VALID_DATA << std::endl;
    }
}

int main() {
    // Example of using default filename constant
    //std::string filename = DEFAULT_TEMPERATURE_FILE;

    std::string filename;

    std::cout << "Enter the name of the temperature data file: ";
    std::cin >> filename;
    
    readTemperatureData(filename);

    return 0;
}
