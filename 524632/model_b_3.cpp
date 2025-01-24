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
        pos.x += (rand() % 3) - 1;
        pos.y += (rand() % 3) - 1;
        pos.x = max(0, min(pos.x, maxX));
        pos.y = max(0, min(pos.y, maxY));
        if (!animalFilter[species] && behaviorFilter != "all") printf("[%s] Movement: (%d,%d) -> (%d,%d)\n", species.c_str(), oldPos.x, oldPos.y, pos.x, pos.y);
    }

    void checkInteraction(Animal& other) {
        int distance = abs(pos.x - other.pos.x) + abs(pos.y - other.pos.y);
        if (distance <= 2) {
            if (isPredator && other.isPredator) {
                behavior = "territorial";
            } else if (isPredator && !other.isPredator) {
                behavior = "hunting";
            } else {
                behavior = "alert";
            }

            if (!animalFilter[species] && behaviorFilter != "all" && (behaviorFilter == "alert" || behaviorFilter == behavior)) printf("(%s) INTERACTION: %s encountered %s at (%d,%d)\n", behavior.c_str(), species.c_str(), other.species.c_str(), pos.x, pos.y);
            if (animalFilter[species] || animalFilter[other.species]) printf("!! ANIMAL CLOSEBY: %s at (%d,%d)!!\n", other.species.c_str(), pos.x, pos.y);
        }
    }

};

class WildlifeSimulation {
private:
    vector<Animal> animals;
    const int PARK_SIZE = 20;
    unordered_map<string, bool> animalFilter = {
        {"Lion", false}, {"Zebra", false}, {"Elephant", false}, {"Tiger", false}
    };
    string behaviorFilter = "all";

public:
    void initialize() {
        animals.push_back(Animal("Lion", {5, 5}, true));
        animals.push_back(Animal("Zebra", {10, 10}));
        animals.push_back(Animal("Elephant", {15, 15}));
        animals.push_back(Animal("Tiger", {18, 12}, true));
    }

    void setAnimalFilter(string animal, bool isFiltered) {
        animalFilter[animal] = isFiltered;
    }

    void setBehaviorFilter(string behavior) {
        behaviorFilter = behavior;
    }
    
    void runSimulation(int steps) {
        for (int i = 0; i < steps; i++) {
            printf("\n=== Simulation Step %d ===\n", i);

            for (auto& animal : animals) {
                animal.move(PARK_SIZE, PARK_SIZE);
                for (auto& other : animals) {
                    if (&animal != &other) {
                        animal.checkInteraction(other);
                    }
                }
            }
        }
    }
};

int main() {
    srand(time(0));
    WildlifeSimulation sim;
    sim.initialize();

    // Focusing on "Lion" behaviors and "Elephant" species movements
    sim.setAnimalFilter("Elephant", false);
    sim.setBehaviorFilter("hunting");