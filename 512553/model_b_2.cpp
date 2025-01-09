#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>

using namespace std;

const int MAX_CURRENCIES = 10;
const double INF = numeric_limits<double>::max();

int main() {
    int numCurrencies = 3; // Number of currencies: USD, EUR, GBP
    double exchangeRates[MAX_CURRENCIES][MAX_CURRENCIES] = {
        {0, 1.1586, 1.6939},
        {0, 0, 1.4600},
        {0, 0, 0}
    };

    double initialCapital = 1000000.0; // Initial capital in USD

    // Initialize the distance matrix (all paths are initially infinite)
    double distance[MAX_CURRENCIES][MAX_CURRENCIES] = {
        {0, -log(exchangeRates[0][1]), -log(exchangeRates[0][2])},
        {INF, 0, -log(exchangeRates[1][2])},
        {INF, INF, 0}
    };

    // Floyd-Warshall algorithm to find the maximum profit path for all pairs of currencies
    for (int k = 0; k < numCurrencies; k++) {
        for (int i = 0; i < numCurrencies; i++) {
            for (int j = 0; j < numCurrencies; j++) {
                distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j]);
            }
        }
    }

    // Check for arbitrage opportunity (profit > 0)
    double maxProfit = -INF;
    int profitablePath[MAX_CURRENCIES];

    for (int i = 0; i < numCurrencies; i++) {
        profitablePath[i] = i;
        double profit = distance[0][i];
        if (profit > 0) {
            maxProfit = max(maxProfit, profit);
        }
    }

    // Output results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Initial Capital (USD): " << initialCapital << std::endl;
    if (maxProfit > 0) {
        std::cout << "Arbitrage opportunity exists!" << std::endl;
        std::cout << "Maximum Profit: " << exp(-maxProfit) * initialCapital - initialCapital << std::endl;
        std::cout << "Profit Path: ";
        for (int i = 0; i < numCurrencies; i++) {
            std::cout << "Currency " << profitablePath[i] + 1 << " -> ";
        }
        std::cout << "Currency " << profitablePath[0] + 1 << std::endl;
    } else {
        std::cout << "No arbitrage opportunity." << std::endl;
    }

    return 0;
}