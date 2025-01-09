#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stack>
#include <set>
#include <memory>

// Struct to represent an edge in the graph
struct Edge {
    std::string to;
    double rate;
};

// Define an Observer interface
class Observer {
public:
    virtual void update(const std::string& cycle, double finalAmount, double profit) = 0;
    virtual ~Observer() = default;
};

// Define the ArbitrageDetectionSystem, which is the Subject
class ArbitrageDetectionSystem {
private:
    std::set<Observer*> observers;
    std::unordered_map<std::string, std::vector<Edge>> graph = {
        {"USD", {{"EUR", 1.1586}, {"GBP", 1.6939}}},
        {"EUR", {{"GBP", 1.4600}, {"USD", 1 / 1.1586}}},
        {"GBP", {{"USD", 1 / 1.6939}, {"EUR", 1 / 1.4600}}}
    };

public:
    // Attach an observer
    void attach(Observer* observer) {
        observers.insert(observer);
    }

    // Detach an observer
    void detach(Observer* observer) {
        observers.erase(observer);
    }

    // Perform arbitrage detection and notify observers
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

        // Notify all observers
        for (const auto& observer : observers) {
            observer->update(std::string(cycle.rbegin(), cycle.rend()), bestRate[start], bestRate[start] - initialCapital);
        }
    }
};

// Define a concrete Observer class: ArbitragePrinter
class ArbitragePrinter : public Observer {
public:
    void update(const std::string& cycle, double finalAmount, double profit) override {
        std::cout << "Printer Output: " << std::endl;
        std::cout << "Arbitrage Cycle: " << cycle << std::endl;
        std::cout << "Final Amount (USD): " << finalAmount << std::endl;
        std::cout << "Profit: " << profit << std::endl;
    }
};

int main() {
    double initialCapital = 1000000.0; // Initial capital in USD

    ArbitrageDetectionSystem system;

    // Attach the ArbitragePrinter observer
    ArbitragePrinter printer;
    system.attach(&printer);

    system.detectArbitrage("USD", initialCapital);

    return 0;
}