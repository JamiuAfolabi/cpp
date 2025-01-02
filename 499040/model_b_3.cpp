#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <map>

class Nail {
public:
    std::string material;
    double length;
    bool isShaped;
    bool isHeated;
    bool isInspected;

    Nail(std::string m, double l) : material(m), length(l), isShaped(false), isHeated(false), isInspected(false) {}

    void shape() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isShaped = true;
        log({
            {"process", "shape"},
            {"status", "completed"},
            {"nail_material", material},
            {"nail_length", length}
        });
    }

    void heat() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isHeated = true;
        log({
            {"process", "heat"},
            {"status", "completed"},
            {"nail_material", material},
            {"nail_length", length}
        });
    }

    void inspect() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isInspected = true;
        log({
            {"process", "inspect"},
            {"status", "completed"},
            {"nail_material", material},
            {"nail_length", length},
            {"nail_quality", isInspected ? "ready" : "defective"}
        });
    }

private:
    void log(const std::map<std::string, std::any>& data) {
        std::cout << getCurrentTime() << " - ";
        for (const auto& [key, value] : data) {
            std::cout << key << ": " << std::any_cast<std::string>(value) << ", ";
        }
        std::cout << std::endl;
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

class NailProductionProcess {
private:
    std::vector<Nail> nails;

public:
    void inputMaterials() {
        log({
            {"process", "material_input"},
            {"status", "started"},
            {"nail_count", nails.size()}
        });
        nails.emplace_back("Steel", 5.0);
        nails.emplace_back("Steel", 3.5);
        nails.emplace_back("Steel", 4.0);
        log({
            {"process", "material_input"},
            {"status", "completed"},
            {"nail_count", nails.size()}
        });
    }

    void shapeNails() {
        log({
            {"process", "shape_nails"},
            {"status", "started"}
        });
        for (auto& nail : nails) {
            nail.shape();
        }
        log({
            {"process", "shape_nails"},
            {"status", "completed"}
        });
    }

    void heatNails() {
        log({
            {"process", "heat_nails"},
            {"status", "started"}
        });
        for (auto& nail : nails) {
            nail.heat();
        }