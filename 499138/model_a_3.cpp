#include <iostream>
#include <vector>
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

void controlAcUsingIfElse(const std::vector<double>& temperatures) {
    AirConditioner ac;
    for (double temp : temperatures) {
        TemperatureCategory category = categorizeTemperature(temp);
        if (category == TemperatureCategory::VERY_LOW) {
            ac.setVeryLow();
        } else if (category == TemperatureCategory::LOW) {
            ac.setLow();
        } else if (category == TemperatureCategory::MEDIUM) {
            ac.setMedium();
        } else if (category == TemperatureCategory::HIGH) {
            ac.setHigh();
        } else { // VERY_HIGH
            ac.setVeryHigh();
        }
    }
}

void controlAcUsingSwitch(const std::vector<double>& temperatures) {
    AirConditioner ac;
    for (double temp : temperatures) {
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
}

int main() {
    const size_t size = 1'000'000;
    std::vector<double> temperatures(size, 0); // Initialize with 0 for simplicity
    
    // Fill the vector with random temperature values for testing
    for (size_t i = 0; i < size; ++i) {
        temperatures[i] = static_cast<double>(rand() % 451) / 10.0; // Random temperatures from 0.0 to 45.0
    }

    // Benchmarking if-else implementation
    auto start = std::chrono::high_resolution_clock::now();
    controlAcUsingIfElse(temperatures);
    auto end = std::chrono::high_resolution_clock::now();
    auto ifElseDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // Benchmarking switch implementation
    start = std::chrono::high_resolution_clock::now();
    controlAcUsingSwitch(temperatures);
    end = std::chrono::high_resolution_clock::now();
    auto switchDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Print execution times
    std::cout << "If-Else Duration: " << ifElseDuration << " microseconds" << std::endl;
    std::cout << "Switch Duration: " << switchDuration << " microseconds" << std::endl;

    return 0;
}