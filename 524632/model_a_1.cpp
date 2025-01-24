#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

class Animal {
protected:
    std::string species;
    int x, y;  // Position in the park

public:
    Animal(std::string species, int startX, int startY) 
        : species(species), x(startX), y(startY) {}

    void move() {
        // Randomly move the animal by -1, 0, or +1 in both x and y directions
        x += (rand() % 3) - 1;
        y += (rand() % 3) - 1;
        printf("%s moved to (%d, %d)\n", species.c_str(), x, y);
    }

    void interact(Animal& other) {
        printf("%s interacted with %s at (%d, %d)\n", species.c_str(), other.species.c_str(), other.x, other.y);
    }

    const std::string getSpecies() const { return species; }
    int getX() const { return x; }
    int getY() const { return y; }
};

class Deer : public Animal {
public:
    Deer(int startX, int startY) : Animal("Deer", startX, startY) {}
};

class Wolf : public Animal {
public:
    Wolf(int startX, int startY) : Animal("Wolf", startX, startY) {}
};

class Environment {
private:
    std::vector<Animal*> animals;
    
public:
    void addAnimal(Animal* animal) {
        animals.push_back(animal);
    }

    void simulateMovement() {
        for (auto& animal : animals) {
            animal->move();
            checkInteractions(*animal);
        }
    }

    void checkInteractions(Animal& animal) {
        for (auto& other : animals) {
            if (&animal != other && animal.getX() == other->getX() && animal.getY() == other->getY()) {
                animal.interact(*other);
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  // Seed for random number generation

    Environment park;

    // Adding animals to the park
    park.addAnimal(new Deer(0, 0));
    park.addAnimal(new Deer(1, 1));
    park.addAnimal(new Wolf(0, 1));

    // Simulate movements
    for (int i = 0; i < 10; ++i) {
        printf("\n--- Simulation Step %d ---\n", i + 1);
        park.simulateMovement();
    }

    // Clean up memory
    for (auto animal : park.animals) {
        delete animal;
    }

    return 0;
}