#include <iostream>
#include <string>

// Function to categorize temperature
std::string categorizeTemperature(float temperature) {
    if (temperature < 16) {
        return "cold";
    } else if (temperature >= 16 && temperature <= 30) {
        return "okay";
    } else {
        return "high";
    }
}

int main() {
    // Example inputs for testing
    float temperature1 = 15.0; // Input for 'cold'
    float temperature2 = 25.0; // Input for 'okay'
    
    std::cout << "Temperature: " << temperature1 << "°C - Category: " << categorizeTemperature(temperature1) << std::endl;
    std::cout << "Temperature: " << temperature2 << "°C - Category: " << categorizeTemperature(temperature2) << std::endl;

    return 0;
}