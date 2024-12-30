#include <iostream>
#include <string>

// Constants for temperature thresholds
constexpr float COLD_THRESHOLD = 16.0f;
constexpr float HIGH_THRESHOLD = 30.0f;

// Function to categorize temperature
std::string categorizeTemperature(float temperature) {
    if (temperature < COLD_THRESHOLD) {
        return "cold";
    } else if (temperature >= COLD_THRESHOLD && temperature <= HIGH_THRESHOLD) {
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