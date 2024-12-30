#include<bits/stdc++.h>
using namespace std;

#define NUM_SAMPLES 20

// Function to generate a random float within the range [-spread, spread]
float random_fluctuation(float spread) {
    static uniform_real_distribution<> dist(-spread, spread);
    return dist(random_device());
}

// Function to simulate the stock price sequence
float generate_stock_price(float initial_price, float volatility) {
    float current_price = initial_price;
    float prices[NUM_SAMPLES];

    prices[0] = current_price;

    for (int i = 1; i < NUM_SAMPLES; i++) {
        float daily_return = 1.0 + (random_fluctuation(volatility) / 100.0);
        current_price *= daily_return;
        prices[i] = current_price;
    }

    // display prices
    cout << "Generated Stock Prices: \n";
    for(int i = 0; i < NUM_SAMPLES; i++) {
        cout << prices[i] << endl;
    }

    return prices;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    float initial_price = 100.0; // Set the initial stock price
    float volatility = 1.5; // Annual volatility (e.g., 1.5% for daily fluctuations)

    // Generate the stock price sequence and store them in the array 'prices'
    float* prices = generate_stock_price(initial_price, volatility);


    // Now you have the stock price data in the 'prices' array and can perform further analysis.

    // For example, let's calculate a simple 5-day moving average:
    float moving_average[NUM_SAMPLES - 4];
    for (int i = 4; i < NUM_SAMPLES; i++) {
        float sum = 0.0;
        for (int j = 0; j < 5; j++) {
            sum += prices[i - j];
        }
        moving_average[i - 4] = sum / 5.0;
        cout << "Simple Moving Average " << i+1 << ": " << moving_average[i-4] << endl;
    }

    return 0;
}