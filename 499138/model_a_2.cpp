#include <iostream>
#include <vector>
#include <string>

class AirConditioner {
public:
    enum class Mode { OFF, COOL, VERY_COOL };
    Mode mode = Mode::OFF;
    int fanSpeed = 0;

    void setVeryLow() {
        mode = Mode::COOL;
        fanSpeed = 2;
    }

    void setLow() {
        mode = Mode::COOL;
        fanSpeed = 3;
    }

    void setMedium() {
        mode = Mode::COOL;
        fanSpeed = 4;
    }

    void setHigh() {
        mode = Mode::VERY_COOL;
        fanSpeed = 5;
    }

    void setVeryHigh() {
        mode = Mode::VERY_COOL;
        fanSpeed = 5;
    }

    void displaySettings() {
        if (mode == Mode::COOL)
            std::cout << "AC Mode: Cool, Fan Speed: " << fanSpeed << std::endl;
        else
            std::cout << "AC Mode: Very Cool, Fan Speed: " << fanSpeed << std::endl;
    }
};

enum class TemperatureCategory { VERY_LOW, LOW, MEDIUM, HIGH, VERY_HIGH };

TemperatureCategory categorizeTemperature(double temperature) {
    if (temperature <= 20) {
        return TemperatureCategory::VERY_LOW;
    } else if (temperature <= 25) {
        return TemperatureCategory::LOW;
    } else if (temperature <= 30) {
        return TemperatureCategory::MEDIUM;
    } else if (temperature <= 35) {
        return TemperatureCategory::HIGH;
    } else {
        return TemperatureCategory::VERY_HIGH;
    }
}

int main() {
    AirConditioner ac;
    std::vector<double> temperatures = { 22.5, 28.7, 32.0, 18.4, 36.9, 24.0, 26.6, 19.0 };

    for (double temp : temperatures) {
        TemperatureCategory category = categorizeTemperature(temp);
        std::cout << "Temperature: " << temp << "°C, Category: ";

        switch (category) {
            case TemperatureCategory::VERY_LOW:
                std::cout << "Very Low" << std::endl;
                ac.setVeryLow();
                break;
            case TemperatureCategory::LOW:
                std::cout << "Low" << std::endl;
                ac.setLow();
                break;
            case TemperatureCategory::MEDIUM:
                std::cout << "Medium" << std::endl;
                ac.setMedium();
                break;
            case TemperatureCategory::HIGH:
                std::cout << "High" << std::endl;
                ac.setHigh();
                break;
            case TemperatureCategory::VERY_HIGH:
                std::cout << "Very High" << std::endl;
                ac.setVeryHigh();
                break;
        }
        ac.displaySettings();
    }
    return 0;
}