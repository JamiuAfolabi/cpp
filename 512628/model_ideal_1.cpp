#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>

class TwinTowerAirFilter {
public:
    // Method to generate the data samples
    void generateDataSamples(int sampleSize, int intervalMs) {
        double operationalTime = 0.0; // Initialize operational time in seconds
        for (int i = 0; i < sampleSize; ++i) {
            auto data = collectData(operationalTime);
            displayData(data);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            operationalTime += intervalMs / 1000.0; // Increment operational time by the interval in seconds
        }
    }

private:
    struct FilterData {
        double pressure;            // Pressure (Pa)
        double flowRate;           // Flow Rate (m³/h)
        double filterEfficiency;   // Filter Efficiency (%)
        double temperature;        // Temperature (°C)
        double humidity;           // Humidity (%)
        double powerConsumption;   // Power Consumption (W)
        std::string maintenanceStatus; // Maintenance Status
        double operationalTime;    // Operational Time (seconds)
    };

    FilterData collectData(double operationalTime) {
        FilterData data;

        // Simulate pressure in the range of 5000 to 20000 Pa
        data.pressure = randomValue(5000, 20000);
        // Simulate flow rate in the range of 100 to 2000 m³/h
        data.flowRate = randomValue(100, 2000);
        // Simulate filter efficiency in the range of 90 to 99.9%
        data.filterEfficiency = randomValue(90.0, 99.9);
        // Simulate temperature in the range of 15 to 30°C
        data.temperature = randomValue(15.0, 30.0);
        // Simulate humidity in the range of 30 to 70%
        data.humidity = randomValue(30.0, 70.0);
        // Simulate power consumption in the range of 100 to 1000 W
        data.powerConsumption = randomValue(100, 1000);
        // Simulate maintenance status: "OK" or "Needs Maintenance"
        data.maintenanceStatus = (rand() % 5 == 0) ? "Needs Maintenance" : "OK"; // 20% chance of needing maintenance
        // Use passed operational time
        data.operationalTime = operationalTime;

        return data;
    }

    void displayData(const FilterData& data) {
        std::cout << std::left << std::setw(25) << "Pressure:" 
                  << std::setw(15) << data.pressure << " Pa"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Flow Rate:" 
                  << std::setw(15) << data.flowRate << " m³/h"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Filter Efficiency:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << data.filterEfficiency << "%"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Temperature:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << data.temperature << "°C"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Humidity:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << data.humidity << "%"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Power Consumption:" 
                  << std::setw(15) << data.powerConsumption << " W"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Maintenance Status:" 
                  << std::setw(15) << data.maintenanceStatus
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Operational Time:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << data.operationalTime << " seconds"
                  << std::endl;

        std::cout << "------------------------------------------------------------" << std::endl;
    }

    double randomValue(double min, double max) {
        return min + static_cast<double>(rand()) / (RAND_MAX / (max - min));
    }
};

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    TwinTowerAirFilter airFilter;
    airFilter.generateDataSamples(20, 2000); // 20 samples every 2000 ms (2 seconds)

    return 0;
}
