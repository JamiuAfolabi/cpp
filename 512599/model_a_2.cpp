#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>

// Constants for brightness and contrast adjustments
const int LOW_LIGHT_BRIGHTNESS = 30; // Low light brightness level
const int LOW_LIGHT_CONTRAST = 20;    // Low light contrast level
const int NORMAL_BRIGHTNESS = 100;     // Normal brightness level
const int NORMAL_CONTRAST = 100;       // Normal contrast level
const int HIGH_LIGHT_BRIGHTNESS = 255; // High light brightness level
const int HIGH_LIGHT_CONTRAST = 200;    // High light contrast level

class BH1750 {
public:
    BH1750() {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random noise
        lightIntensity = 0.0;
    }

    // Simulate the light intensity readings
    void simulateLightIntensity() {
        int environment = std::rand() % 4; // 0: No light, 1: Shade, 2: Indoor, 3: Direct Sunlight
        switch (environment) {
            case 0: lightIntensity = 0.0; break;         // No light
            case 1: lightIntensity = 100.0 + randomNoise(); break; // Shade
            case 2: lightIntensity = 300.0 + randomNoise(); break; // Indoor lighting
            case 3: lightIntensity = 1000.0 + randomNoise(); break; // Direct sunlight
        }
    }

    // Simulate sensor noise
    float randomNoise() {
        return (std::rand() % 20) - 10; // Noise between -10 and +10 lux
    }

    // Simulate I2C communication
    void sendData(float intensity) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Sending data via I2C: " << intensity << " lux" << std::endl;
    }

    // Energy-efficient mode simulation
    void enterLowPowerMode() {
        std::cout << "Entering low power mode..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Display integrated data
    void displayData(float intensity) {
        std::cout << "Current light intensity: " << intensity << " lux" << std::endl;
    }

    // Adjust brightness and contrast based on lux levels
    void adjustDisplayBrightnessAndContrast(float intensity) {
        if (intensity < 100) { // Low Light
            setDisplaySettings(LOW_LIGHT_BRIGHTNESS, LOW_LIGHT_CONTRAST);
        } else if (intensity >= 100 && intensity <= 500) { // Medium Light
            setDisplaySettings(NORMAL_BRIGHTNESS, NORMAL_CONTRAST);
        } else { // High Light
            setDisplaySettings(HIGH_LIGHT_BRIGHTNESS, HIGH_LIGHT_CONTRAST);
        }
    }

    // Simulate setting display brightness and contrast
    void setDisplaySettings(int brightness, int contrast) {
        std::cout << "Adjusting display - Brightness: " << brightness 
                  << ", Contrast: " << contrast << std::endl;
    }

    void runSimulation() {
        while (true) {
            simulateLightIntensity();
            displayData(lightIntensity);
            adjustDisplayBrightnessAndContrast(lightIntensity); // Adjust display settings based on lux
            sendData(lightIntensity);
            enterLowPowerMode();
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Simulate cycle duration
        }
    }

private:
    float lightIntensity; // Simulated light intensity in lux
};

int main() {
    BH1750 sensor;
    sensor.runSimulation();
    return 0;
}