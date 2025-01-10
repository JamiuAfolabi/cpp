#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>

class TwinTowerAirFilter {
public:
    // Constructor and Destructor
    TwinTowerAirFilter(int sampleSize) : sampleSize(sampleSize) {
        data = new FilterData[sampleSize]; // Dynamically allocate memory for data samples
    }

    ~TwinTowerAirFilter() {
        delete[] data; // Free dynamically allocated memory
    }

    // Method to generate the data samples
    void generateDataSamples(int intervalMs) {
        double operationalTime = 0.0; // Initialize operational time in seconds
        for (int i = 0; i < sampleSize; ++i) {
            collectData(&data[i], operationalTime); // Store data using pointer
            displayData(&data[i]); // Display data using pointer
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            operationalTime += intervalMs / 1000.0; // Increment operational time by the interval in seconds
        }
    }

private:
    struct FilterData {
        double pressure;            // Pressure (Pa)
        double flowRate;           // Flow Rate (m³/h)
        double filterEfficiency;   // Filter Efficiency (%)
        double temperature;         // Temperature (°C)
        double humidity;           // Humidity (%)
        double powerConsumption;   // Power Consumption (W)
        std::string maintenanceStatus; // Maintenance Status
        double operationalTime;    // Operational Time (seconds)
    };

    FilterData* data; // Pointer to dynamic array
    int sampleSize;

    void collectData(FilterData* ptr, double operationalTime) {
        // Use the pointer to access and modify the data directly
        ptr->pressure = randomValue(5000, 20000);
        ptr->flowRate = randomValue(100, 2000);
        ptr->filterEfficiency = randomValue(90.0, 99.9);
        ptr->temperature = randomValue(15.0, 30.0);
        ptr->humidity = randomValue(30.0, 70.0);
        ptr->powerConsumption = randomValue(100, 1000);
        ptr->maintenanceStatus = (rand() % 5 == 0) ? "Needs Maintenance" : "OK"; // 20% chance of needing maintenance
        ptr->operationalTime = operationalTime;
    }

    void displayData(FilterData* ptr) {
        std::cout << std::left << std::setw(25) << "Pressure:" 
                  << std::setw(15) << ptr->pressure << " Pa"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Flow Rate:" 
                  << std::setw(15) << ptr->flowRate << " m³/h"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Filter Efficiency:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << ptr->filterEfficiency << "%"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Temperature:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << ptr->temperature << "°C"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Humidity:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << ptr->humidity << "%"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Power Consumption:" 
                  << std::setw(15) << ptr->powerConsumption << " W"
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Maintenance Status:" 
                  << std::setw(15) << ptr->maintenanceStatus
                  << std::endl;

        std::cout << std::left << std::setw(25) << "Operational Time:" 
                  << std::setw(15) << std::fixed << std::setprecision(1) << ptr->operationalTime << " seconds"
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

    int samples = 20; // Number of samples
    int intervalMs = 2000; // Interval in milliseconds

    TwinTowerAirFilter airFilter(samples);
    airFilter.generateDataSamples(intervalMs); // Call to generate data samples

    return 0;
}