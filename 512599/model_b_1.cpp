
#include <iostream>
#include <cstdlib>   // For rand() function
#include <ctime>     // For seeding rand()

class BH1750Sensor {
private:
    static const int MAX_LUX = 65535;    // Maximum measured lux value
    static const int NOISE_LEVEL = 50;    // Lux value of sensor noise
    int luxValue;

public:
    BH1750Sensor() {
        // Initialize lux to a random value within the sensor range
        luxValue = rand() % MAX_LUX;
    }

    // Function to simulate light intensity in different environments
    void simulateLightIntensity(int environment) {
        switch (environment) {
            case 1: // Bright sunlight
                luxValue = (rand() % 40000) + 20000;
                break;
            case 2: // Indoor office
                luxValue = (rand() % 10000) + 5000;
                break;
            case 3: // Shaded area
                luxValue = (rand() % 2000) + 500;
                break;
            case 4: // Dark room
                luxValue = (rand() % 500) + 100;
                break;
            default:
                luxValue = rand() % MAX_LUX; // Random environment
                break;
        }

        // Add sensor noise
        luxValue += (rand() % NOISE_LEVEL) - NOISE_LEVEL / 2;
        if (luxValue < 0) {
            luxValue = 0;
        }
    }

    int getLuxValue() const {
        return luxValue;
    }

    // Simple I2C communication simulation (read data)
    void readI2C() {
        std::cout << "Reading I2C data... ";
        simulateLightIntensity(1); // For demonstration, reading in bright sunlight
        std::cout << "Lux value: " << luxValue << " lx" << std::endl;
    }

    // Calculate and print energy efficiency (assuming constant measurement rate)
    void calculateEnergyEfficiency() {
        const float MEASUREMENT_TIME = 1.0f;   // Time taken for one measurement (s)
        const float CURRENT_CONSUMPTION = 0.1f; // Typical current consumption (mA) during measurement
        
        float energyConsumed = CURRENT_CONSUMPTION * MEASUREMENT_TIME / 1000.0f; // in joules

        std::cout << "Energy efficiency: ";
        // Assuming the sensor measures continuously, we'll only count the useful part
        float efficiency = (float)luxValue / energyConsumed / 1000.0f; // /1000 to convert lx to (lx/J)
        std::cout << efficiency << " lx/J" << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    BH1750Sensor sensor;
    std::cout << "Initial Lux Value: " << sensor.getLuxValue() << " lx" << std::endl;

    // Simulate reading sensor data over I2C
    sensor.readI2C();

    // Simulate different environments
    std::cout << "\nSimulating different environments:\n";
    sensor.simulateLightIntensity(2); // Indoor office
    std::cout << "Lux value in Indoor office: " << sensor.getLuxValue() << " lx" << std::endl;

    sensor.simulateLightIntensity(4); // Dark room
    std::cout << "Lux value in Dark room: " << sensor.getLuxValue() << " lx" << std::endl;
    
    sensor.calculateEnergyEfficiency();

    return 0;
}
