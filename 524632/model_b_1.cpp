#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
#include <string>
#include <unordered_map>
using namespace std;

class Animal {
public:
    string name;
    int x, y; // Position in the park
    int moveRange;
    int direction; // 0: Up, 1: Right, 2: Down, 3: Left

    Animal(string name, int x, int y, int moveRange) : name(name), x(x), y(y), moveRange(moveRange) {}

    void move() {
        direction = (direction + 1) % 4; // Turn right randomly
        switch (direction) {
            case 0:
                y--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
        }
        x = std::max(0, std::min(100, x)); // Prevent going out of bounds (100x100 park)
        y = std::max(0, std::min(100, y));
    }

    void interact(Animal &other) {
        if (name == other.name) {
            printf("[Event] %s meets another %s.\n", name.c_str(), name.c_str());
        } else {
            printf("[Event] %s encounters a %s.\n", name.c_str(), other.name.c_str());
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Create animals with random initial positions and movement ranges
    unordered_map<string, Animal> animals = {
        {"Lion", 20, 30, 15},
        {"Zebra", 45, 10, 10},
        {"Snake", 70, 60, 5},
        {"Bird", 10, 80, 20}
    };

    int days = 30;
    for (int day = 1; day <= days; day++) {
        printf("\nDay %d:\n", day);
        for (auto &[name, animal] : animals) {
            animal.move();
            printf("%s is at position (%d, %d)\n", animal.name.c_str(), animal.x, animal.y);

            // Check for interactions
            for (auto &[otherName, otherAnimal] : animals) {
                if (name != otherName && std::abs(animal.x - otherAnimal.x) <= 1 && std::abs(animal.y - otherAnimal.y) <= 1) {
                    animal.interact(otherAnimal);
                }
            }
        }
    }

    return 0;
}