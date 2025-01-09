#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stack>
#include <memory>
#include <algorithm>
#include <sstream>

// Observer interface
class Observer {
public:
    virtual void update(const std::string& message) = 0;
};

// Concrete Observer: Display updates
class DisplayObserver : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

// Subject interface
class Subject {
private:
    std::vector<Observer*> observers;

public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(const std::string& message) {
        for (Observer* observer : observers) {
            observer->update(message);
        }
    }
};

// Struct to represent an edge in the graph
struct Edge {
    std::string to;
    double rate;
};

// Arbitrage Detection System
class ArbitrageDetectionSystem : public Subject {
private:
    // Graph representation
    struct Graph {
        std::unordered_map<std::string, std::vector<Edge>> graph = {
            {"USD", {{"EUR", 1.1586}, {"GBP", 1.6939}}},
            {"EUR", {{"GBP", 1.4600}, {"USD", 1 / 1.1586}}},
            {"GBP", {{"USD", 1 / 1.6939}, {"EUR", 1 / 1.4600}}}
        };
    };

    std::unique_ptr<Graph> graphPtr;

    ArbitrageDetectionSystem() : graphPtr(std::make_unique<Graph>()) {}

public:
    static ArbitrageDetectionSystem& getInstance() {
        static ArbitrageDetectionSystem instance;
        return instance;
    }

    void detectArbitrage(const std::string& start, double initialCapital) {
        const auto& graph = graphPtr->graph;
        
        // Best rates and path tracking
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
            notify("No arbitrage opportunity.");
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

        std::ostringstream message;
        message << "Arbitrage opportunity exists!\nCycle: ";
        while (!cycle.empty()) {
            message << cycle.top() << " ";
            cycle.pop();
        }

        message << "\nProfit: " << std::fixed << std::setprecision(2) << (bestRate[start] - initialCapital);
        notify(message.str());
    }
};

int main() {
    DisplayObserver display;
    ArbitrageDetectionSystem& system = ArbitrageDetectionSystem::getInstance();
    system.attach(&display);

    double initialCapital = 1000000.0; // Initial capital in USD
    system.detectArbitrage("USD", initialCapital);

    return 0;
}