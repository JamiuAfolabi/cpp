#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stack>

// Struct to represent an edge in the graph
struct Edge {
    std::string to;
    double rate;
};

// Function to detect arbitrage and print the path
void detectArbitrage(
    const std::unordered_map<std::string, std::vector<Edge>>& graph, 
    const std::string& start, 
    double initialCapital) 
{
    // Initialize best rates and predecessors for path tracking
    std::unordered_map<std::string, double> bestRate;
    std::unordered_map<std::string, std::string> predecessor;

    // Initialize best rates with a very small value
    for (const auto& node : graph) {
        bestRate[node.first] = std::numeric_limits<double>::lowest();
    }

    // Starting currency has the initial capital
    bestRate[start] = initialCapital;

    // Relax edges |V| - 1 times (Bellman-Ford main loop)
    size_t numCurrencies = graph.size();
    for (size_t i = 0; i < numCurrencies - 1; ++i) {
        for (const auto& [from, edges] : graph) {
            for (const auto& edge : edges) {
                if (bestRate[from] > 0) {
                    double potentialRate = bestRate[from] / edge.rate;
                    if (potentialRate > bestRate[edge.to]) {
                        bestRate[edge.to] = potentialRate;
                        predecessor[edge.to] = from;
                    }
                }
            }
        }
    }

    // Check for arbitrage cycle
    std::string arbitrageStart;
    for (const auto& [from, edges] : graph) {
        for (const auto& edge : edges) {
            if (bestRate[from] > 0) {
                double potentialRate = bestRate[from] / edge.rate;
                if (potentialRate > bestRate[edge.to]) {
                    arbitrageStart = edge.to;
                }
            }
        }
    }

    // If no arbitrage cycle found
    if (arbitrageStart.empty()) {
        std::cout << "No arbitrage opportunity." << std::endl;
        return;
    }

    // Trace the arbitrage cycle
    std::string current = arbitrageStart;
    std::stack<std::string> cycle;
    for (size_t i = 0; i < numCurrencies; ++i) {
        current = predecessor[current];
    }
    std::string cycleStart = current;

    do {
        cycle.push(current);
        current = predecessor[current];
    } while (current != cycleStart);
    cycle.push(current);

    // Calculate maximum amount
    double finalAmount = initialCapital;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Arbitrage opportunity exists!" << std::endl;
    std::cout << "Cycle: ";
    while (!cycle.empty()) {
        std::cout << cycle.top() << " ";
        cycle.pop();
    }
    std::cout << std::endl;

    // Display maximum final amount
    std::cout << "Final Amount (USD): " << bestRate[start] << std::endl;
    std::cout << "Profit: " << bestRate[start] - initialCapital << std::endl;
}

int main() {
    double initialCapital = 1000000.0; // Initial capital in USD

    // Graph of exchange rates
    std::unordered_map<std::string, std::vector<Edge>> graph = {
        {"USD", {{"EUR", 1.1586}, {"GBP", 1.6939}}},
        {"EUR", {{"GBP", 1.4600}, {"USD", 1 / 1.1586}}},
        {"GBP", {{"USD", 1 / 1.6939}, {"EUR", 1 / 1.4600}}}
    };

    // Detect arbitrage and print results
    detectArbitrage(graph, "USD", initialCapital);

    return 0;
}
