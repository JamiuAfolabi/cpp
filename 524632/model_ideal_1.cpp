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
    
    Animal(std::string sp, Position p) : species(sp), pos(p) {
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

        printf("[%s] Movement: (%d,%d) -> (%d,%d)\n", 
               species.c_str(), oldPos.x, oldPos.y, pos.x, pos.y);
    }

    void checkInteraction(Animal& other) {
        int distance = std::abs(pos.x - other.pos.x) + std::abs(pos.y - other.pos.y);
        if (distance <= 2) {
            printf("INTERACTION: %s encountered %s at (%d,%d)\n", 
                   species.c_str(), other.species.c_str(), pos.x, pos.y);
            behavior = "alert";
        }
    }
};

class WildlifeSimulation {
private:
    std::vector<Animal> animals;
    const int PARK_SIZE = 20;

public:
    void initialize() {
        animals.push_back(Animal("Lion", {5, 5}));
        animals.push_back(Animal("Zebra", {10, 10}));
        animals.push_back(Animal("Elephant", {15, 15}));
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