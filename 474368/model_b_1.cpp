
#include <iostream>
#include <string>

enum class TemperatureCategory { 
    COLD, 
    OKAY, 
    HIGH 
};

TemperatureCategory categorizeTemperature(float temperature)
{
    if (temperature < 16.0) {
        return TemperatureCategory::COLD;
    } else if (temperature <= 30.0) {
        return TemperatureCategory::OKAY;
    } else {
        return TemperatureCategory::HIGH;
    }
}

int main()
{
    float temperatureInput1 = 25.6;  // Example input 1
    float temperatureInput2 = 10.5; // Example input 2
 
    TemperatureCategory categorizedTemp1 = categorizeTemperature(temperatureInput1);
    TemperatureCategory categorizedTemp2 = categorizeTemperature(temperatureInput2);
  
    std::cout << "Temperature Category for input 1: ";
    switch (categorizedTemp1)
    {
        case TemperatureCategory::COLD: std::cout << "cold" << std::endl; break;
        case TemperatureCategory::OKAY: std::cout << "okay" << std::endl; break;
        case TemperatureCategory::HIGH: std::cout << "high" << std::endl; break;
    }

    std::cout << "Temperature Category for input 2: ";
    switch (categorizedTemp2)
    {  
        case TemperatureCategory::COLD: std::cout << "cold" << std::endl; break;
        case TemperatureCategory::OKAY: std::cout << "okay" << std::endl; break;
        case TemperatureCategory::HIGH: std::cout << "high" << std::endl; break;
    }

    return 0;
}