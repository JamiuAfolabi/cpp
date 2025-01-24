#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <random>
#include <string>

using namespace std;

struct Position {
    int x, y;
};

class Animal {
public:
    string species;
    Position pos;
    string behavior;
    string territory;
    bool isPredator;

    Animal(string sp, Position p, bool isPred = false) : species(sp), pos(p), isPredator(isPred) {
        behavior = "normal";
        territory = "neutral";
    }

    void move(int maxX, int maxY) {
        Position oldPos = pos;
        pos.x += (rand() % 3) - 1;  // Move -1, 0, or 1 in x
        pos.y += (rand() % 3) - 1;  // Move -1, 0, or 1 in y
        
        // Keep within bounds
        pos.x = std::max(0, std::min(pos.x, maxX));
        pos.y = std::max(0, std::min(pos.y, maxY));
        
        printf("[%s] Movement: (%d,%d) -> (%d,%d)\n", species.c_str(), oldPos.x, oldPos.y, pos.x, pos.y);
    }

    void checkInteraction(Animal& other, const string& filterSpecies, const string& filterBehavior) {
        int distance = abs(pos.x - other.pos.x) + abs(pos.y - other.pos.y);
        if (distance <= 2) {
            if (isPredator && other.isPredator) {
                behavior = "territorial";
                logMessage("!! TERRITORY CONFLICT: %s predators at (%d,%d)!?!\n",
                           filterSpecies, filterBehavior);
            } else if (isPredator && !other.isPredator) {
                behavior = "alert";
                logMessage("!!! PREDATOR-PREY CONFLICT: %s spotted %s at (%d,%d)!!!\n",
                           filterSpecies, filterBehavior);
            } else {
                logMessage("INTERACTION: %s encountered %s at (%d,%d)\n",
                           species.c_str(), other.species.c_str(), pos.x, pos.y);
            }
        }
    }

    void logMessage(const char* format, const string& filterSpecies, const string& filterBehavior) {
        if ((filterSpecies == species || filterSpecies.empty()) &&
            (filterBehavior == behavior || filterBehavior.empty())) {
            printf(format, species.c_str(), pos.x, pos.y);
        }
    }
};

class WildlifeSimulation {
private:
    vector<Animal> animals;
    const int PARK_SIZE = 20;

public:
    void initialize() {
        animals.push_back(Animal("Lion", {5, 5}, true));    // Predator
        animals.push_back(Animal("Zebra", {10, 10}));      // Prey
        animals.push_back(Animal("Elephant", {15, 15}));   // Not involved
        animals.push_back(Animal("Tiger", {18, 12}, true)); // Predator
    }

    void setSimulationFilters(string speciesFilter, string behaviorFilter) {
        for (int i = 0; i < 20; i++) {
            printf("\n=== Simulation Step %d ===\n", i);

            for (auto& animal : animals) {
                animal.move(PARK_SIZE, PARK_SIZE);

                for (auto& other : animals) {
                    if (&animal != &other) {
                        animal.checkInteraction(other, speciesFilter, behaviorFilter);
                    }
                }
            }
        }
    }

    void runSimulationWithFilter(int steps, string speciesFilter = "", string behaviorFilter = "") {
        setSimulationFilters(speciesFilter, behaviorFilter);
    }
};

int main() {
    srand(time(0));
    WildlifeSimulation sim;
    sim.initialize();

    // Set filters for specific animal and behavior. Example: "Lion" and "alert"
    string speciesFilter = "Lion";       // Specify "" for all species
    string behaviorFilter = "alert";     // Specify "" for all behaviors

    sim.runSimulationWithFilter(20, speciesFilter, behaviorFilter);
    return 0;
}