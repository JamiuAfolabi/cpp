#include <iostream>
#include <vector>
#include <string>

class AirConditioner {
public:
    enum class Mode {OFF, COOL, VERY_COOL};
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
        if(mode == Mode::COOL)
            std::cout << "AC Mode: Cool, Fan Speed: " << fanSpeed << std::endl;
        else
            std::cout << "AC Mode: Very Cool, Fan Speed: " << fanSpeed << std::endl;
    }
};

std::string categorizeTemperature(double temperature) {
    if (temperature <= 20) {
        return "Very Low";
    } else if (temperature <= 25) {
        return "Low";
    } else if (temperature <= 30) {
        return "Medium";
    } else if (temperature <= 35) {
        return "High";
    } else {
        return "Very High";
    }
}

int main() {
    AirConditioner ac;
    std::vector<double> temperatures = {22.5, 28.7, 32.0, 18.4, 36.9, 24.0, 26.6, 19.0};

    for (double temp : temperatures) {
        std::string category = categorizeTemperature(temp);
        std::cout << "Temperature: " << temp << "°C, Category: " << category << std::endl;

        if (category == "Very Low") {
            ac.setVeryLow();
        } else if (category == "Low") {
            ac.setLow();
        } else if (category == "Medium") {
            ac.setMedium();
        } else if (category == "High") {
            ac.setHigh();
        } else if (category == "Very High") {
            ac.setVeryHigh();
        }
        ac.displaySettings();
    }
    return 0;
}