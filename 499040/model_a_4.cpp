#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

class Nail {
public:
    std::string material;
    double length;
    bool isShaped;
    bool isHeated;
    bool isInspected;
    double productionTime; // Time taken to produce this nail

    Nail(std::string m, double l) : material(m), length(l), isShaped(false), isHeated(false), isInspected(false), productionTime(0.0) {}

    void shape() {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate shaping time
        isShaped = true;
        auto end = std::chrono::high_resolution_clock::now();
        productionTime += std::chrono::duration<double, std::milli>(end - start).count(); // Add shaping time
        log("Nail shaped", {{"material", material}, {"length", std::to_string(length)}});
    }

    void heat() {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate heating time
        isHeated = true;
        auto end = std::chrono::high_resolution_clock::now();
        productionTime += std::chrono::duration<double, std::milli>(end - start).count(); // Add heating time
        log("Nail heated", {{"material", material}, {"length", std::to_string(length)}});
    }

    void inspect() {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate inspection time
        isInspected = true;
        auto end = std::chrono::high_resolution_clock::now();
        productionTime += std::chrono::duration<double, std::milli>(end - start).count(); // Add inspection time
        log("Nail inspected", {{"material", material}, {"length", std::to_string(length)}});
    }

    double getProductionTime() const {
        return productionTime; // Return total production time for this nail
    }

private:
    void log(const std::string& message, const std::vector<std::pair<std::string, std::string>>& details) {
        std::cout << getCurrentTime() << " - " << formatLog(message, details) << std::endl;
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string formatLog(const std::string& message, const std::vector<std::pair<std::string, std::string>>& details) {
        std::ostringstream oss;
        oss << "{\"event\": \"" << message << "\"";
        for (const auto& detail : details) {
            oss << ", \"" << detail.first << "\": \"" << detail.second << "\"";
        }
        oss << "}";
        return oss.str();
    }
};

class NailProductionProcess {
private:
    std::vector<Nail> nails;

public:
    void inputMaterials() {
        log("Inputting materials", {});
        nails.emplace_back("Steel", 5.0);
        nails.emplace_back("Steel", 3.5);
        nails.emplace_back("Steel", 4.0);
        log("Materials inputted", {{"count", std::to_string(nails.size())}});
    }

    void shapeNails() {
        log("Shaping nails", {});
        for (auto& nail : nails) {
            nail.shape();
        }
    }

    void heatNails() {
        log("Heating nails", {});
        for (auto& nail : nails) {
            nail.heat();
        }
    }

    void inspectNails() {
        log("Inspecting nails", {});
        for (auto& nail : nails) {
            nail.inspect();
            if (nail.isInspected) {
                log("Nail ready for packaging", {{"length", std::to_string(nail.length)}});
            }
        }
    }

    void runProductionProcess() {
        inputMaterials();
        shapeNails();
        heatNails();
        inspectNails();
        
        // Calculate and log the average production time for a single nail
        double totalProductionTime = 0.0;
        for (const auto& nail : nails) {
            totalProductionTime += nail.getProductionTime();
        }
        
        if (!nails.empty()) {
            double averageTimePerNail = totalProductionTime / nails.size();
            log("Average production time per nail", {{"time_ms", std::to_string(averageTimePerNail)}});
        }
        
        log("Production process completed", {});
    }

private:
    void log(const std::string& message, const std::vector<std::pair<std::string, std::string>>& details) {
        std::cout << getCurrentTime() << " - " << formatLog(message, details) << std::endl;
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string formatLog(const std::string& message, const std::vector<std::pair<std::string, std::string>>& details) {
        std::ostringstream oss;
        oss << "{\"event\": \"" << message << "\"";
        for (const auto& detail : details) {
            oss << ", \"" << detail.first << "\": \"" << detail.second << "\"";
        }
        oss << "}";
        return oss.str();
    }
};

int main() {
    NailProductionProcess production;
    production.runProductionProcess();
    return 0;
}