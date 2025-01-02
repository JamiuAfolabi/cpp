
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

class AirConditioner {
public:
    enum class Mode { OFF, COOL, VERY_COOL };
    Mode mode = Mode::OFF;
    int fanSpeed = 0;

    void setVeryLow() { mode = Mode::COOL; fanSpeed = 2; }
    void setLow() { mode = Mode::COOL; fanSpeed = 3; }
    void setMedium() { mode = Mode::COOL; fanSpeed = 4; }
    void setHigh() { mode = Mode::VERY_COOL; fanSpeed = 5; }
    void setVeryHigh() { mode = Mode::VERY_COOL; fanSpeed = 5; }
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

void controlACWithIfElse(AirConditioner& ac, double temp) {
    std::string category = categorizeTemperature(temp);
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
}

void controlACWithSwitch(AirConditioner& ac, double temp) {
    TemperatureCategory category = categorizeTemperature(temp);
    switch (category) {
        case TemperatureCategory::VERY_LOW:
            ac.setVeryLow();
            break;
        case TemperatureCategory::LOW:
            ac.setLow();
            break;
        case TemperatureCategory::MEDIUM:
            ac.setMedium();
            break;
        case TemperatureCategory::HIGH:
            ac.setHigh();
            break;
        case TemperatureCategory::VERY_HIGH:
            ac.setVeryHigh();
            break;
    }
}

int main() {
    AirConditioner ac;
    size_t numTemperatures = 1'000'000;
    std::vector<double> temperatures(numTemperatures);

    // Generate random temperature readings
    for (size_t i = 0; i < numTemperatures; ++i) {
        temperatures[i] = 18.0 + 18.0 * (std::rand() / static_cast<double>(RAND_MAX));
    }

    // Benchmark with if-else
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (double temp : temperatures) {
            controlACWithIfElse(ac, temp);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Execution time with if-else: " << duration.count() << " microseconds" << std::endl;
    }

  // Benchmark with switch
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (double temp : temperatures) {
            controlACWithSwitch(ac, temp);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Execution time with switch: " << duration.count() << " microseconds" << std::endl;
    }

    return 0;
}

