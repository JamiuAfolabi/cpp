#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stack>
#include <mutex>

class ArbitrageDetector {
public:
    // Get the single instance of the ArbitrageDetector
    static ArbitrageDetector& getInstance() {
        static ArbitrageDetector instance; // Lazy initialization
        return instance;
    }

    // Public function to add an exchange rate and detect arbitrage
    void addExchangeRate(const std::string& from, const std::string& to, double rate) {
        graph[from].emplace_back(Edge{to, rate});
    }

    void detectArbitrage(const std::string& start, double initialCapital) {
        // Initialize best rates and predecessors for path tracking
        std::unordered_map<std::string, double> bestRate;
        std::unordered_map<std::string, std::string> predecessor;

        for (const auto& node : graph) {
            bestRate[node.first] = std::numeric_limits<double>::lowest();
        }

        bestRate[start] = initialCapital;

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

        if (arbitrageStart.empty()) {
            std::cout << "No arbitrage opportunity." << std::endl;
            return;
        }

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

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Arbitrage opportunity exists!" << std::endl;
        std::cout << "Cycle: ";
        while (!cycle.empty()) {
            std::cout << cycle.top() << " ";
            cycle.pop();
        }
        std::cout << std::endl;
        std::cout << "Final Amount (USD): " << bestRate[start] << std::endl;
        std::cout << "Profit: " << bestRate[start] - initialCapital << std::endl;
    }

private:
    struct Edge {
        std::string to;
        double rate;
    };

    // Private constructor for Singleton
    ArbitrageDetector() {}
    
    // Prevent copying
    ArbitrageDetector(const ArbitrageDetector&) = delete;
    ArbitrageDetector& operator=(const ArbitrageDetector&) = delete;

    // Graph representation
    std::unordered_map<std::string, std::vector<Edge>> graph;
};

// Test program
int main() {
    double initialCapital = 1000000.0; // Initial capital in USD

    // Get the ArbitrageDetector instance
    ArbitrageDetector& detector = ArbitrageDetector::getInstance();

    // Add exchange rates to the graph
    detector.addExchangeRate("USD", "EUR", 1.1586);
    detector.addExchangeRate("EUR", "GBP", 1.4600);
    detector.addExchangeRate("USD", "GBP", 1.6939);

    // Detect arbitrage
    detector.detectArbitrage("USD", initialCapital);

    return 0;
}