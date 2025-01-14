#include <iostream>
#include <vector>
#include <cstdlib>  // For rand() and srand() functions
#include <ctime>   // For seeding rand()
#include <chrono>

using namespace std;

const int NUM_SAMPLES = 20;
const int SAMPLE_INTERVAL_SECONDS = 2;

// Function to generate random data within specified ranges
double generateRandomData(double min, double max) {
    return (double)rand() / (double)RAND_MAX * (max - min) + min;
}

// Function to generate random integer within specified ranges
int generateRandomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

int main() {
    vector<double> pressure(NUM_SAMPLES);
    vector<double> flowRate(NUM_SAMPLES);
    vector<double> filterEfficiency(NUM_SAMPLES);
    vector<double> temperature(NUM_SAMPLES);
    vector<double> humidity(NUM_SAMPLES);
    vector<double> powerConsumption(NUM_SAMPLES);
    vector<string> maintenanceStatus(NUM_SAMPLES);
    vector<int> operationalTime(NUM_SAMPLES);

    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_SAMPLES; ++i) {
        // Simulate data generation every 2 seconds
        auto currentTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
        int currentTimeSeconds = duration.count();
        
        // Generate data for each metric
        pressure[i] = generateRandomData(1000.0, 1200.0);
        flowRate[i] = generateRandomData(150.0, 200.0);
        filterEfficiency[i] = generateRandomData(80.0, 95.0);
        temperature[i] = generateRandomData(20.0, 28.0);
        humidity[i] = generateRandomData(40.0, 60.0);
        powerConsumption[i] = generateRandomData(50.0, 80.0);
        maintenanceStatus[i] = generateRandomInt(0, 1) == 0 ? "OK" : "Needs Maintenance";
        operationalTime[i] = currentTimeSeconds;

        // Sleep for the remaining time until the next sample interval
        auto sleepDuration = chrono::seconds(SAMPLE_INTERVAL_SECONDS - (currentTimeSeconds % SAMPLE_INTERVAL_SECONDS));
        std::this_thread::sleep_for(sleepDuration);
    }

    // Display the generated data
    cout << "\nGenerated Data for Twin Tower Air Filter System:\n";
    cout << "-----------------------------------------------\n";
    cout << "Pressure (Pa) | Flow Rate (m³/h) | Filter Efficiency (%) | Temperature (°C) | Humidity (%) | Power Consumption (W) | Maintenance Status | Operational Time (seconds)\n";
    cout << "-----------------------------------------------\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        cout << fixed << setprecision(2) << pressure[i] << " | " << fixed << setprecision(2) << flowRate[i] << " | " << fixed << setprecision(1) << filterEfficiency[i] << " | " << fixed << setprecision(1) << temperature[i] << " | " << fixed << setprecision(1) << humidity[i] << " | " << fixed << setprecision(2) << powerConsumption[i] << " | " << maintenanceStatus[i] << " | " << operationalTime[i] << endl;
    }

    return 0;
}