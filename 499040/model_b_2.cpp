#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <locale>
#include <sstream>

// ... (existing code)
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

std::string getCurrentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time_since_epoch = now.time_since_epoch();
    seconds sec = duration_cast<seconds>(time_since_epoch);
    microseconds usec = duration_cast<microseconds>(time_since_epoch - sec);
    auto milliseconds = usec.count() / 1000;
    std::tm t = localtime(&sec.count());

    std::ostringstream oss;
    oss << std::put_time(&t, "%Y-%m-%d %H:%M:%S.%03d");
    return oss.str();
}

// ... (existing code)

void Nail::shape() {
    // Simulate shaping process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isShaped = true;
    std::cout << getCurrentTimestamp() << ": Nail shaped." << std::endl;
}

void Nail::heat() {
    // Simulate heating process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isHeated = true;
    std::cout << getCurrentTimestamp() << ": Nail heated." << std::endl;
}

void Nail::inspect() {
    // Simulate inspection process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isInspected = true;
    std::cout << getCurrentTimestamp() << ": Nail inspected." << std::endl;
}


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
// ... (existing code)

int main() {
    NailProductionProcess production;
    production.runProductionProcess();
    return 0;
}