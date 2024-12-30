#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <memory> // Include for smart pointers

void generateStockPrices(double* prices, double initialPrice, int numSamples) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    prices[0] = initialPrice;

    for (int i = 1; i < numSamples; ++i) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0; // Random fluctuation between -1.0 and +1.0
        prices[i] = prices[i - 1] + fluctuation;

        // Ensure the stock price doesn't go below zero
        if (prices[i] < 0) {
            prices[i] = 0.0;
        }
    }
}

void calculateMovingAverage(double* prices, double* movingAverages, int numSamples, int period) {
    if (period <= 0) {
        std::cerr << "Error: Period must be greater than zero." << std::endl;
        return;
    }
    
    for (int i = 0; i < numSamples; ++i) {
        if (i < period - 1) {
            movingAverages[i] = 0.0; // Not enough data for moving average
        } else {
            double sum = 0.0;
            // Calculate the sum of the last 'period' prices using pointer arithmetic
            for (int j = 0; j < period; ++j) {
                sum += *(prices + i - j); // Access prices using pointer arithmetic
            }
            movingAverages[i] = sum / period; // Calculate average
        }
    }
}

int main() {
    const int NUM_SAMPLES = 20;
    // Use smart pointers for dynamic memory allocation
    std::unique_ptr<double[]> stockPrices(new double[NUM_SAMPLES]);
    std::unique_ptr<double[]> movingAverages(new double[NUM_SAMPLES]);
    
    double initialPrice = 100.0; // Starting price
    int movingAveragePeriod = 5; // Define the period for moving average

    generateStockPrices(stockPrices.get(), initialPrice, NUM_SAMPLES);

    std::cout << std::fixed << std::setprecision(2); // Set precision for floating-point numbers
    std::cout << "Simulated Stock Prices:\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Price " << i + 1 << ": $" << stockPrices[i] << std::endl;
    }

    calculateMovingAverage(stockPrices.get(), movingAverages.get(), NUM_SAMPLES, movingAveragePeriod);

    std::cout << "\nMoving Averages (Period = " << movingAveragePeriod << "):\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Moving Average " << i + 1 << ": $" << movingAverages[i] << std::endl;
    }

    // No need to manually delete memory; unique_ptr will handle it automatically

    return 0;
}