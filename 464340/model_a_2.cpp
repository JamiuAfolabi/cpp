#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void originalImplementation(const vector<double>& prices) {
    // Original version
    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        double priceDifference = currentPrice - previousPrice;
        // cout << "Day " << i << ": Price Difference = " << priceDifference << endl; // Commented out
    }

    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        // cout << "Day " << i << ": Previous Price = " << previousPrice 
        //      << ", Current Price = " << currentPrice << endl; // Commented out
    }
}

void optimizedImplementation(const vector<double>& prices) {
    // Optimized version with loop fusion
    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        double priceDifference = currentPrice - previousPrice;
        // cout << "Day " << i << ": Price Difference = " << priceDifference 
        //      << ", Previous Price = " << previousPrice 
        //      << ", Current Price = " << currentPrice << endl; // Commented out
    }
}

int main() {
    vector<double> prices = {100.5, 101.0, 102.5, 103.0, 104.5, 105.0};
    
    // Measure execution time for original implementation
    auto startOriginal = chrono::high_resolution_clock::now();
    originalImplementation(prices);
    auto endOriginal = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationOriginal = endOriginal - startOriginal;

    // Measure execution time for optimized implementation
    auto startOptimized = chrono::high_resolution_clock::now();
    optimizedImplementation(prices);
    auto endOptimized = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationOptimized = endOptimized - startOptimized;

    // Print the execution time for both implementations
    cout << "Original implementation time: " << durationOriginal.count() << " ms" << endl;
    cout << "Optimized implementation time: " << durationOptimized.count() << " ms" << endl;

    return 0;
}