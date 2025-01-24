#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <random>

struct Position {
    int x, y;
};

class Animal {
public:
    std::string species;
    Position pos;
    std::string behavior;
    std::string territory;
    bool isPredator;

    Animal(std::string sp, Position p, bool predator) 
        : species(sp), pos(p), isPredator(predator) {
        behavior = "normal";
        territory = "neutral";  // Could evolve into more specific territory management
    }

    void move(int maxX, int maxY) {
        Position oldPos = pos;
        pos.x += (rand() % 3) - 1;  // Move -1, 0, or 1 in x
        pos.y += (rand() % 3) - 1;  // Move -1, 0, or 1 in y
        
        // Keep within bounds
        pos.x = std::max(0, std::min(pos.x, maxX));
        pos.y = std::max(0, std::min(pos.y, maxY));

        // Log movements only when changing territory
        if (pos.x != oldPos.x || pos.y != oldPos.y) {
            printf("[%s] Movement: (%d,%d) -> (%d,%d)\n", 
                   species.c_str(), oldPos.x, oldPos.y, pos.x, pos.y);
        }
    }

    void checkInteraction(Animal& other) {
        int distance = std::abs(pos.x - other.pos.x) + std::abs(pos.y - other.pos.y);
        if (distance <= 2) {
            // Check for predator-prey interactions
            if (isPredator && !other.isPredator) {
                printf("PREDATOR-PREY CONFLICT: %s (Predator) encountered %s (Prey) at (%d,%d)\n", 
                       species.c_str(), other.species.c_str(), pos.x, pos.y);
            } else if (!isPredator && other.isPredator) {
                printf("PREDATOR-PREY CONFLICT: %s (Prey) encountered %s (Predator) at (%d,%d)\n", 
                       species.c_str(), other.species.c_str(), pos.x, pos.y);
            }
            behavior = "alert";  // Animals alert upon encountering each other
        }
    }
};

class WildlifeSimulation {
private:
    std::vector<Animal> animals;
    const int PARK_SIZE = 20;

public:
    void initialize() {
        animals.push_back(Animal("Lion", {5, 5}, true)); // Predator
        animals.push_back(Animal("Zebra", {10, 10}, false)); // Prey
        animals.push_back(Animal("Elephant", {15, 15}, false)); // Prey
    }

    void runSimulation(int steps) {
        for(int i = 0; i < steps; i++) {
            printf("\n=== Simulation Step %d ===\n", i);
            
            for(auto& animal : animals) {
                animal.move(PARK_SIZE, PARK_SIZE);
                
                // Check interactions with other animals
                for(auto& other : animals) {
                    if(&animal != &other) {
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
    sim.runSimulation(10);
    return 0;
}