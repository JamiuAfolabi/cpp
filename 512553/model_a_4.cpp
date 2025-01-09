#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stack>
#include <functional>
#include <memory>

// Struct to represent an edge in the graph
struct Edge {
    std::string to;
    double rate;
};

// Observer interface
class Observer {
public:
    virtual void update(const std::string& message) = 0;
};

// Concrete observer for displaying results
class Display : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class ArbitrageDetectionSystem {
private:
    struct Graph {
        std::unordered_map<std::string, std::vector<Edge>> graph = {
            {"USD", {{"EUR", 1.1586}, {"GBP", 1.6939}}},
            {"EUR", {{"GBP", 1.4600}, {"USD", 1 / 1.1586}}},
            {"GBP", {{"USD", 1 / 1.6939}, {"EUR", 1 / 1.4600}}}
        };
    };

    std::unique_ptr<Graph> graphPtr;
    std::vector<std::shared_ptr<Observer>> observers;

    void initializeGraph() {
        graphPtr = std::make_unique<Graph>();
    }

    void notifyObservers(const std::string& message) {
        for (const auto& observer : observers) {
            observer->update(message);
        }
    }

    ArbitrageDetectionSystem() = default;

public:
    static ArbitrageDetectionSystem& getInstance() {
        static ArbitrageDetectionSystem instance;
        return instance;
    }

    void addObserver(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void detectArbitrage(const std::string& start, double initialCapital) {
        initializeGraph();
        const auto& graph = graphPtr->graph;

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
            notifyObservers("No arbitrage opportunity.");
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

        double finalAmount = initialCapital;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Arbitrage opportunity exists!\nCycle: ";
        while (!cycle.empty()) {
            oss << cycle.top() << " ";
            cycle.pop();
        }
        oss << "\nFinal Amount (USD): " << bestRate[start] 
            << "\nProfit: " << bestRate[start] - initialCapital;

        notifyObservers(oss.str());
    }
};

int main() {
    double initialCapital = 1000000.0; // Initial capital in USD

    // Create an observer for display
    std::shared_ptr<Observer> display = std::make_shared<Display>();

    // Add observer to the arbitrage detection system
    ArbitrageDetectionSystem::getInstance().addObserver(display);

    // Detect arbitrage using the Singleton instance
    ArbitrageDetectionSystem::getInstance().detectArbitrage("USD", initialCapital);

    return 0;
}