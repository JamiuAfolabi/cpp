#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int NUM_SAMPLES = 20;

// Function to generate stock prices
void generateStockPrices(double prices[], double initialPrice) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    prices[0] = initialPrice;

    for (int i = 1; i < NUM_SAMPLES; ++i) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0; // Random fluctuation between -1.0 and +1.0
        prices[i] = prices[i - 1] + fluctuation;

        // Ensure the stock price doesn't go below zero
        if (prices[i] < 0) {
            prices[i] = 0.0;
        }
    }
}

// Function to calculate moving average using pointer arithmetic
void calculateMovingAverage(double prices[], double movingAverages[], int period) {
    if (period <= 0) {
        std::cerr << "Error: Period must be greater than zero." << std::endl;
        return;
    }
    
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        if (i < period - 1) {
            movingAverages[i] = 0.0; // Not enough data for moving average
        } else {
            double sum = 0.0;
            // Calculate the sum of the last 'period' prices
            for (int j = 0; j < period; ++j) {
                sum += *(prices + i - j); // Using pointer arithmetic
            }
            movingAverages[i] = sum / period; // Calculate average
        }
    }
}

int main() {
    double stockPrices[NUM_SAMPLES];
    double movingAverages[NUM_SAMPLES];
    double initialPrice = 100.0; // Starting price
    int movingAveragePeriod = 5; // Define the period for moving average

    generateStockPrices(stockPrices, initialPrice);

    std::cout << std::fixed << std::setprecision(2); // Set precision for floating-point numbers
    std::cout << "Simulated Stock Prices:\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Price " << i + 1 << ": $" << stockPrices[i] << std::endl;
    }

    calculateMovingAverage(stockPrices, movingAverages, movingAveragePeriod);

    std::cout << "\nMoving Averages (Period = " << movingAveragePeriod << "):\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Moving Average " << i + 1 << ": $" << movingAverages[i] << std::endl;
    }

    return 0;
}