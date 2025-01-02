#include <iostream>
#include <vector>

class AirConditioner {
public:
    void setMode(const std::string &mode) {
        currentMode = mode;
        std::cout << "Air Conditioner is set to " << currentMode << " mode." << std::endl;
    }

private:
    std::string currentMode;
};

void categorizeAndControlAirConditioner(const std::vector<double>& temperatures) {
    AirConditioner ac;
    
    for (double temp : temperatures) {
        if (temp < 0) {
            ac.setMode("Very Low - Heating Required");
        } else if (temp >= 0 && temp < 15) {
            ac.setMode("Low - Mild Heating");
        } else if (temp >= 15 && temp < 25) {
            ac.setMode("Medium - Comfortable");
        } else if (temp >= 25 && temp < 35) {
            ac.setMode("High - Cooling Optional");
        } else {
            ac.setMode("Very High - Cooling Required");
        }
    }
}

int main() {
    std::vector<double> temperatureReadings = { -5.0, 10.0, 20.0, 30.0, 40.0 };

    categorizeAndControlAirConditioner(temperatureReadings);

    return 0;
}