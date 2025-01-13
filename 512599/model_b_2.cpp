#include <iostream>
#include <cstdlib>
#include <ctime>

enum class LightLevel {
    LOW,
    MEDIUM,
    HIGH
};

const int MAX_BRIGHTNESS = 255;
const int MAX_CONTRAST = 255;

struct DisplaySettings {
    int brightness;
    int contrast;
};

// Function to calculate brightness and contrast based on lux levels
DisplaySettings calculateDisplaySettings(float lux) {
    LightLevel lightLevel;
    if (lux < 100.0f) {
        lightLevel = LightLevel::LOW;
    } else if (lux <= 500.0f) {
        lightLevel = LightLevel::MEDIUM;
    } else {
        lightLevel = LightLevel::HIGH;
    }

    DisplaySettings settings;
    switch (lightLevel) {
        case LightLevel::LOW:
            settings.brightness = int(0.5 * MAX_BRIGHTNESS);
            settings.contrast = int(0.5 * MAX_CONTRAST);
            std::cout << "Low light: Adjusting brightness to " << settings.brightness << " and contrast to " << settings.contrast << std::endl;
            break;
        case LightLevel::MEDIUM:
            settings.brightness = MAX_BRIGHTNESS;
            settings.contrast = MAX_CONTRAST;
            std::cout << "Medium light: Adjusting brightness to " << settings.brightness << " and contrast to " << settings.contrast << std::endl;
            break;
        case LightLevel::HIGH:
            settings.brightness = int(1.2 * MAX_BRIGHTNESS);
            settings.contrast = int(1.2 * MAX_CONTRAST);
            std::cout << "High light: Adjusting brightness to " << settings.brightness << " and contrast to " << settings.contrast << std::endl;
            break;
    }

    return settings;
}

int main() {
    float lux = 200.0f;
    DisplaySettings settings = calculateDisplaySettings(lux);
    return 0;
}
