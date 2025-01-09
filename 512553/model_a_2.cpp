#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <iomanip>
#include <string>
#include <unordered_map>

struct Edge {
    int from;
    int to;
    double rate;
};

class Arbitrage {
public:
    void addEdge(int from, int to, double rate) {
        edges.push_back({from, to, rate});
    }

    void findArbitrage(int startCurrency, double initialCapital) {
        int n = 3; // Number of currencies (USD, EUR, GBP)
        std::vector<double> profit(n, -std::numeric_limits<double>::infinity());
        std::vector<int> parent(n, -1);
        profit[startCurrency] = initialCapital; // Starting point

        // Bellman-Ford algorithm
        for (int i = 0; i < n - 1; ++i) {
            for (const auto& edge : edges) {
                if (profit[edge.from] > 0) {
                    double newProfit = profit[edge.from] / edge.rate;
                    if (newProfit > profit[edge.to]) {
                        profit[edge.to] = newProfit;
                        parent[edge.to] = edge.from;
                    }
                }
            }
        }

        // Check for arbitrage opportunities
        for (const auto& edge : edges) {
            if (profit[edge.from] > 0) {
                double newProfit = profit[edge.from] / edge.rate;
                if (newProfit > profit[edge.to]) {
                    std::cout << "Arbitrage opportunity detected!" << std::endl;
                    printArbitragePath(parent, edge.from);
                    return;
                }
            }
        }

        std::cout << "No arbitrage opportunity found." << std::endl;
    }

private:
    std::vector<Edge> edges;

    void printArbitragePath(const std::vector<int>& parent, int startingCurrency) {
        std::vector<int> path;
        int current = startingCurrency;

        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }

        std::cout << "Arbitrage path (from last to first): ";
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            std::cout << currencyNames[*it] << (it + 1 != path.rend() ? " -> " : "");
        }
        std::cout << std::endl;
    }

    const std::unordered_map<int, std::string> currencyNames = {
        {0, "USD"},
        {1, "EUR"},
        {2, "GBP"}
    };
};

int main() {
    Arbitrage arb;
    double initialCapital = 1000000.0; // Initial capital in USD

    // Adding edges (currency conversions)
    arb.addEdge(0, 1, 1.1586); // USD to EUR
    arb.addEdge(1, 2, 1.4600); // EUR to GBP
    arb.addEdge(0, 2, 1.6939); // USD to GBP

    // Start from USD (0)
    arb.findArbitrage(0, initialCapital);

    return 0;
}