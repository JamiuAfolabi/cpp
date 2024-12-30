#include <iostream>
#include <string>

const float COLD_THRESHOLD = 16.0;
const float HOT_THRESHOLD = 30.0;

// Function to categorize temperature
std::string categorizeTemperature(float temperature) {
    if (temperature < COLD_THRESHOLD) {
        return "cold";
    } else if (temperature <= HOT_THRESHOLD) {
        return "okay";
    } else {
        return "high";
    }
}

int main() {
    // Example inputs for testing
    float temperature1 = 22.0; // Example input: 22 degrees Celsius
    float temperature2 = 35.0; // Example input: 35 degrees Celsius
    
    std::cout << "Temperature: " << temperature1 << "°C - Category: " << categorizeTemperature(temperature1) << std::endl;
    std::cout << "Temperature: " << temperature2 << "°C - Category: " << categorizeTemperature(temperature2) << std::endl;

    return 0;
}