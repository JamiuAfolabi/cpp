#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int NUM_SAMPLES = 20;

void generateStockPrices(double prices[], double initialPrice) {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Set the first stock price to the initial price
    prices[0] = initialPrice;

    // Generate the subsequent stock prices with random fluctuations
    for (int i = 1; i < NUM_SAMPLES; ++i) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0; // Random fluctuation between -1.0 and +1.0
        prices[i] = prices[i - 1] + fluctuation; // Update the price based on fluctuation

        // Ensure the stock price doesn't go below zero
        if (prices[i] < 0) {
            prices[i] = 0.0;
        }
    }
}

int main() {
    double stockPrices[NUM_SAMPLES];
    double initialPrice = 100.0; // Starting price

    generateStockPrices(stockPrices, initialPrice);

    std::cout << std::fixed << std::setprecision(2); // Set precision for floating-point numbers
    std::cout << "Simulated Stock Prices:\n";
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        std::cout << "Price " << i + 1 << ": $" << stockPrices[i] << std::endl;
    }

    return 0;
}