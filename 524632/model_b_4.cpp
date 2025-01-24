#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <random>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

struct Position {
    int x, y;
};

struct Filter {
    set<string> species;
    set<string> behaviors;
    bool showMovement = true;
    bool showInteractions = true;

    bool matchesFilter(const string& animalSpecies, const string& behavior) const {
        bool speciesMatch = species.empty() || species.find(animalSpecies) != species.end();
        bool behaviorMatch = behaviors.empty() || behaviors.find(behavior) != behaviors.end();
        return speciesMatch && behaviorMatch;
    }
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

    void move(int maxX, int maxY, const Filter& filter, ofstream& logFile) {
        Position oldPos = pos;
        pos.x += (rand() % 3) - 1;
        pos.y += (rand() % 3) - 1;
        pos.x = std::max(0, std::min(pos.x, maxX));
        pos.y = std::max(0, std::min(pos.y, maxY));

        if (filter.showMovement && filter.matchesFilter(species, behavior)) {
            logFile << "[%s] Movement: (%d,%d) -> (%d,%d)\n" 
                     << endl;
        }
    }

    void checkInteraction(Animal& other, const Filter& filter, ofstream& logFile) {
        int distance = abs(pos.x - other.pos.x) + abs(pos.y - other.pos.y);
        if (distance <= 2) {
            string oldBehavior = behavior;
            if (isPredator && other.isPredator) {
                behavior = "territorial";
                if (filter.showInteractions &&
                    (filter.matchesFilter(species, behavior) ||
                     filter.matchesFilter(other.species, other.behavior))) {
                    logFile << "!! TERRITORY CONFLICT: %s predators at (%d,%d)!?!\n" 
                             << endl;
                }
            } else if (isPredator && !other.isPredator) {
                behavior = "hunting";
                if (filter.showInteractions &&
                    (filter.matchesFilter(species, behavior) ||
                     filter.matchesFilter(other.species, other.behavior))) {
                    logFile << "!!! PREDATOR-PREY CONFLICT: %s spotted %s at (%d,%d)!!!\n" 
                             << endl;
                }
            } else {
                behavior = "alert";
                if (filter.showInteractions &&
                    (filter.matchesFilter(species, behavior) ||
                     filter.matchesFilter(other.species, other.behavior))) {
                    logFile << "INTERACTION: %s encountered %s at (%d,%d)\n" 
                             << endl;
                }
            }
        }
    }
};

class WildlifeSimulation {
private:
    vector<Animal> animals;
    const int PARK_SIZE = 20;
    Filter filter;
    string logFilePath;

public:
    void setLogFile(string path) { logFilePath = path; }
    void setFilter(const set<string>& species, const set<string>& behaviors) {
        filter.species = species;
        filter.behaviors = behaviors;
    }

    void setLoggingOptions(bool showMovement, bool showInteractions) {
        filter.showMovement = showMovement;
        filter.showInteractions = showInteractions;
    }

    void initialize() {
        animals.push_back(Animal("Lion", {5, 5}, true));
        animals.push_back(Animal("Zebra", {10, 10}));
        animals.push_back(Animal("Elephant", {15, 15}));
        animals.push_back(Animal("Tiger", {18, 12}, true));
    }

    void runSimulation(int steps) {
        ofstream logFile(logFilePath);
        if (!logFile.is_open()) {
            cerr << "Error: Cannot open log file!" << endl;
            return;
        }

        for (int i = 0; i < steps; i++) {
            logFile << "\n=== Simulation Step " << i << " ===\n" << endl;
            for (auto& animal : animals) {
                animal.move(PARK_SIZE, PARK_SIZE, filter, logFile);
                for (auto& other : animals) {
                    if (&animal != &other) {
                        animal.checkInteraction(other, filter, logFile);
                    }
                }
            }
        }
        logFile.close();
    }
};

int main() {
    srand(time(0));
    WildlifeSimulation sim;
    sim.initialize();

    // Example: Filter to monitor only lions and tigers with territorial behavior
    set<string> targetSpecies = {"Lion", "Tiger"};
    set<string> targetBehaviors = {"territorial", "hunting"};
    sim.setFilter(targetSpecies, targetBehaviors);
    sim.setLoggingOptions(true, true);
    sim.setLogFile("animal_log.txt");

    sim.runSimulation(20);
    return 0;
}