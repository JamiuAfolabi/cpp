#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <new> // For operator new


void generateStockPrices(double* prices, double initialPrice, int numSamples) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    prices[0] = initialPrice;

    for (int i = 1; i < numSamples; ++i) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0;
        prices[i] = prices[i - 1] + fluctuation;

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
            movingAverages[i] = 0.0;
        } else {
            double sum = 0.0;
            for (int j = 0; j < period; ++j) {
                sum += *(prices + i - j);
            }
            movingAverages[i] = sum / period;
        }
    }
}

int main() {
    const int NUM_SAMPLES = 20;
    double initialPrice = 100.0;
    int movingAveragePeriod = 5;

    // Dynamically allocate memory for prices and movingAverages
    double* prices = new double[NUM_SAMPLES];
    double* movingAverages = new double[NUM_SAMPLES];

    if (prices == nullptr || movingAverages == nullptr) {
        std::cerr << "Memory allocation failed." << std::endl;
        return 1;
    }

    generateStockPrices(prices, initialPrice, NUM_SAMPLES);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Simulated Stock Prices:\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Price " << i + 1 << ": $" << prices[i] << std::endl;
    }

    calculateMovingAverage(prices, movingAverages, NUM_SAMPLES, movingAveragePeriod);

    std::cout << "\nMoving Averages (Period = " << movingAveragePeriod << "):\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Moving Average " << i + 1 << ": $" << movingAverages[i] << std::endl;
    }

    // Remember to free the dynamically allocated memory
    delete[] prices;
    delete[] movingAverages;

    return 0;
}
 