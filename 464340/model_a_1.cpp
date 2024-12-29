#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<double> prices = {100.5, 101.0, 102.5, 103.0, 104.5, 105.0};

    cout << "Price Differences and Prices:" << endl;
    for (size_t i = 1; i < prices.size(); i++) {
        double previousPrice = prices[i - 1];
        double currentPrice = prices[i];
        double priceDifference = currentPrice - previousPrice;

        // Print both the price difference and the previous/current prices
        cout << "Day " << i << ": Price Difference = " << priceDifference
             << ", Previous Price = " << previousPrice
             << ", Current Price = " << currentPrice << endl;
    }

    return 0;
}