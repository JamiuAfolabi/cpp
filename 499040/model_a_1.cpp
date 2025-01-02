#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

class Nail {
public:
    std::string material;
    double length;
    bool isShaped;
    bool isHeated;
    bool isInspected;
    
    Nail(std::string m, double l) : material(m), length(l), isShaped(false), isHeated(false), isInspected(false) {}

    void shape() {
        // Simulate shaping process
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isShaped = true;
        std::cout << "Nail shaped." << std::endl;
    }

    void heat() {
        // Simulate heating process
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isHeated = true;
        std::cout << "Nail heated." << std::endl;
    }

    void inspect() {
        // Simulate inspection process
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isInspected = true;
        std::cout << "Nail inspected." << std::endl;
    }
};

class NailProductionProcess {
private:
    std::vector<Nail> nails;

public:
    void inputMaterials() {
        std::cout << "Inputting materials..." << std::endl;
        nails.emplace_back("Steel", 5.0);
        nails.emplace_back("Steel", 3.5);
        nails.emplace_back("Steel", 4.0);
        std::cout << "Materials inputted: " << nails.size() << " nails." << std::endl;
    }

    void shapeNails() {
        std::cout << "Shaping nails..." << std::endl;
        for (auto& nail : nails) {
            nail.shape();
        }
    }

    void heatNails() {
        std::cout << "Heating nails..." << std::endl;
        for (auto& nail : nails) {
            nail.heat();
        }
    }

    void inspectNails() {
        std::cout << "Inspecting nails..." << std::endl;
        for (auto& nail : nails) {
            nail.inspect();
            if (nail.isInspected) {
                std::cout << "Nail of length " << nail.length << " is ready for packaging." << std::endl;
            }
        }
    }

    void runProductionProcess() {
        inputMaterials();
        shapeNails();
        heatNails();
        inspectNails();
        std::cout << "Production process completed!" << std::endl;
    }
};

int main() {
    NailProductionProcess production;
    production.runProductionProcess();
    return 0;
}