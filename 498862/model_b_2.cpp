#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int NUM_SAMPLES = 20;

void generateStockPrices(double prices[], double initialPrice) {
    std::srand(static_cast<unsigned int>(std::time(0)));
    prices[0] = initialPrice;

    for (int i = 1; i < NUM_SAMPLES; ++i) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0;
        prices[i] = prices[i - 1] + fluctuation;
        if (prices[i] < 0) {
            prices[i] = 0.0;
        }
    }
}

double calculateMovingAverage(const double* prices, int windowSize, int index) {
    // Check for edge cases
    if (index < 0 || index >= NUM_SAMPLES) {
        std::cout << "Error: Invalid index for moving average calculation." << std::endl;
        return 0.0;
    }

    if (windowSize <= 0 || index < windowSize - 1) {
        std::cout << "Error: Invalid window size. Window size should be positive and less than or equal to data size." << std::endl;
        return 0.0;
    }

    double total = 0.0;
    for (int i = index; i >= index - windowSize + 1; --i) {
        total += prices[i];
    }

    return total / windowSize;
}

int main() {
    double stockPrices[NUM_SAMPLES];
    double initialPrice = 100.0;

    generateStockPrices(stockPrices, initialPrice);

    std::cout << "Simulated Stock Prices:\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Price " << i + 1 << ": $" << stockPrices[i] << std::endl;
    }

    std::cout << "\nSimple Moving Average (5-day window):\n";
    const int windowSize = 5;
    for (int i = windowSize - 1; i < NUM_SAMPLES; ++i) {
        double ma = calculateMovingAverage(stockPrices, windowSize, i);
        std::cout << "MA for Price " << i + 1 << ": $" << ma << std::endl;
    }

    return 0;
}