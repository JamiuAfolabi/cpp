#include <iostream>
#include <ctime>
#include <cstdlib> // For rand() function
#include <vector>

class Nail {
public:
    Nail(double length, double diameter) : length(length), diameter(diameter) {}
    double length;
    double diameter;
};

class NailManufacturingProcess {
public:
    NailManufacturingProcess() {
        std::srand(static_cast<unsigned>(std::time(0)));
    }

    Nail produceNail() {
        // Material Input
        double length = 30.0 + (0.1 * rand() % 101); // Random length between 30.0 and 30.1
        double diameter = 2.0 + (0.01 * rand() % 21); // Random diameter between 2.0 and 2.01

        Nail nail(length, diameter);

        // Shaping
        nail.length = shapeNail(nail.length);
        nail.diameter = shapeNail(nail.diameter);

        // Heating
        nail.length = heatNail(nail.length);

        return nail;
    }

private:
    double shapeNail(double dimension) {
        // Simulate shaping process: Dimension can vary slightly
        double variation = (dimension * 0.005) * (rand() % 201 - 100) / 100.0;
        return dimension + variation;
    }

    double heatNail(double length) {
        // Simulate heating process: Length can slightly increase due to thermal expansion
        double expansion = (length * 0.001) * (rand() % 101 + 1) / 100.0;
        return length + expansion;
    }
};

class FinalInspection {
public:
    bool inspectNail(const Nail& nail) {
        // Simulate inspection: Check if nail meets quality standards
        if (nail.length < 29.5 || nail.length > 30.5 || nail.diameter < 1.9 || nail.diameter > 2.1) {
            std::cout << "Nail rejected: Length=" << nail.length << ", Diameter=" << nail.diameter << "\n";
            return false;
        }
        std::cout << "Nail accepted: Length=" << nail.length << ", Diameter=" << nail.diameter << "\n";
        return true;
    }
};

int main() {
    NailManufacturingProcess manufacturingProcess;
    FinalInspection inspection;

    int numNailsToProduce = 10;

    std::cout << "Digital Twin Simulation of Nail Manufacturing:\n";
    for (int i = 0; i < numNailsToProduce; ++i) {
        Nail nail = manufacturingProcess.produceNail();
        if (!inspection.inspectNail(nail)) {
            --i; // Rejected nail, don't count it
        }
    }

    return 0;
}