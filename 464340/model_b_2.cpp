#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void optimizedLoop(const vector<double>& prices) {
    cout << "Optimized Loop (loop fusion):" << endl;
    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        double priceDifference = currentPrice - previousPrice;
        // cout << "Day " << i << ": Price Difference = " << priceDifference << endl;
    }
}

void unoptimizedLoop(const vector<double>& prices) {
    cout << "\nUnoptimized Loop (two separate loops):" << endl;
    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        double priceDifference = currentPrice - previousPrice;
        // cout << "Day " << i << ": Price Difference = " << priceDifference << endl;
    }

    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];        
        // cout << "Day " << i << ": Previous Price = " << previousPrice 
        //      << ", Current Price = " << currentPrice << endl;
    }
}

int main() {
    const int NUM_ITERATIONS = 10000;
    vector<double> prices(NUM_ITERATIONS + 1);
    // Initialize the prices vector with random values
    for (size_t i = 0; i < prices.size(); i++) {
        prices[i] = 100.0 + double(i);
    }

    // Test 1: Measure performance of the optimized loop
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            optimizedLoop(prices);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Optimized loop execution time: " << duration.count() << " microseconds" << endl;
    }

    // Test 2: Measure performance of the unoptimized loop
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {        
            unoptimizedLoop(prices);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Unoptimized loop execution time: " << duration.count() << " microseconds" << endl;
    }

    return 0;
}