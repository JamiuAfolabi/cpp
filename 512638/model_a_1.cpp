#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>

class TwinTowerAirFilter {
public:
    // Method to generate the data samples
    void generateDataSamples(int sampleSize, int interval) {
        for (int i = 0; i < sampleSize; ++i) {
            auto data = collectData();
            displayData(data);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }

private:
    struct FilterData {
        double pressure;        // Pressure (Pa)
        double flowRate;       // Flow Rate (m³/h)
        double filterEfficiency; // Filter Efficiency (%)
        double temperature;     // Temperature (°C)
        double humidity;        // Humidity (%)
        double powerConsumption; // Power Consumption (W)
        std::string maintenanceStatus; // Maintenance Status
        double operationalTime; // Operational Time (seconds)
    };

    FilterData collectData() {
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
        // Increment operational time (simulated in seconds for simplicity)
        data.operationalTime = interval.count() * (i + 1) / 1000.0;

        return data;
    }

    void displayData(const FilterData& data) {
        std::cout << "Pressure: " << data.pressure << " Pa, "
                  << "Flow Rate: " << data.flowRate << " m³/h, "
                  << "Filter Efficiency: " << std::fixed << std::setprecision(1) << data.filterEfficiency << "%, "
                  << "Temperature: " << std::fixed << std::setprecision(1) << data.temperature << "°C, "
                  << "Humidity: " << std::fixed << std::setprecision(1) << data.humidity << "%, "
                  << "Power Consumption: " << data.powerConsumption << " W, "
                  << "Maintenance Status: " << data.maintenanceStatus << ", "
                  << "Operational Time: " << std::fixed << std::setprecision(1) << data.operationalTime << " seconds" 
                  << std::endl;
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