#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Warehouse {
public:
    Warehouse(size_t size) : inventory(size) {}

    // Function to safely update the inventory position
    void updateItemPosition(size_t index, int newPosition) {
        // Lock the mutex before modifying the inventory
        std::lock_guard<std::mutex> lock(mtx);

        if (index < inventory.size()) {
            inventory[index] = newPosition;
            std::cout << "Item at index " << index << " updated to position " << newPosition << "\n";
        } else {
            std::cout << "Index " << index << " is out of bounds!\n";
        }
    }

    // Function for demonstration purpose
    void printInventory() {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << "Item at index " << i << " is at position " << inventory[i] << "\n";
        }
    }

private:
    std::vector<int> inventory;  // Shared inventory array
    std::mutex mtx;              // Mutex for protecting access to the inventory
};

// Function to simulate a robot updating an item position
void robotUpdate(Warehouse &warehouse, size_t index, int newPosition) {
    warehouse.updateItemPosition(index, newPosition);
}

int main() {
    Warehouse warehouse(10); // Create a warehouse with 10 inventory slots

    // Suppose multiple robots update inventory positions at the same time
    std::thread robot1(robotUpdate, std::ref(warehouse), 3, 15);
    std::thread robot2(robotUpdate, std::ref(warehouse), 5, 20);
    std::thread robot3(robotUpdate, std::ref(warehouse), 3, 30); // This will collide with robot1

    // Wait for the robots to finish their updates
    robot1.join();
    robot2.join();
    robot3.join();

    // Print final inventory state
    warehouse.printInventory();

    return 0;
}