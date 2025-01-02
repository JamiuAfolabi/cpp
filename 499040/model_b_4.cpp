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

    Nail(std::string m, double l) : material(m), length(l), isShaped(false), isHeated(false), isInspected(false) {}

    void shape() {
        startTime = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isShaped = true;
        log("Nail shaped", {{"material", material}, {"length", std::to_string(length)}});
    }

    void heat() {
        startTime = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isHeated = true;
        log("Nail heated", {{"material", material}, {"length", std::to_string(length)}});
    }

    void inspect() {
        startTime = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isInspected = true;
        log("Nail inspected", {{"material", material}, {"length", std::to_string(length)}});
    }

private:
    std::chrono::high_resolution_clock::time_point startTime;
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

    std::string getTimeElapsed() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = endTime - startTime;
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        return std::to_string(nanoseconds) + "ns";
    }
};

class NailProductionProcess {
private:
    std::vector<Nail> nails;
    double totalTimeNails = 0.0;
    int nailCount = 0;

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
            log("Nail shape time: " + nail.getTimeElapsed(), {});
            totalTimeNails += std::stod(nail.getTimeElapsed().substr(0, nail.getTimeElapsed().size() - 2));
            nailCount++;
        }
    }
    
    void heatNails() {
        log("Heating nails", {});
        for (auto& nail : nails) {
            nail.heat();
            log("Nail heat time: " + nail.getTimeElapsed(), {}); 
            totalTimeNails += std::stod(nail.getTimeElapsed().substr(0, nail.getTimeElapsed().size() - 2));
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
        log("Production process completed", {});
        logKPI();
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

    void logKPI() {
        double averageTime = totalTimeNails / nailCount;
        std::cout << "\nKPI: Average time to produce a single nail: " << averageTime << " milliseconds" << std::endl;
    }
};

int main() {
    NailProductionProcess production;
    production.runProductionProcess();
    return 0;
}