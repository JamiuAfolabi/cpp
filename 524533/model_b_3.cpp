#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class Warehouse {
public:
    Warehouse(size_t size) : inventory(size), accessCounts(size) {}

    // Function to safely update the inventory position
    void updateItemPosition(size_t index, int newPosition) {
        std::lock_guard<std::mutex> lock(mtx);
        if (index < inventory.size()) {
            inventory[index] = newPosition;
            std::cout << "Item at index " << index << " updated to position " << newPosition << " (Access count: " << accessCounts[index] + 1 << ")\n";
            accessCounts[index]++;
        } else {
            std::cout << "Index " << index << " is out of bounds!\n";
        }
    }

    // Function to safely swap two item positions
    void swapItems(size_t index1, size_t index2) {
        if (index1 > index2) {
            std::swap(index1, index2);
        }

        std::lock_guard<std::mutex> lock(mtx);

        if (index1 < inventory.size() && index2 < inventory.size()) {
            std::swap(inventory[index1], inventory[index2]);
            std::cout << "Swapped items at index " << index1 << " and " << index2 << "\n";
            accessCounts[index1]++;
            accessCounts[index2]++;
        } else {
            std::cout << "Swap failed: one of the indices " << index1 << " or " << index2 << " is out of bounds!\n";
        }
    }

    // Function for demonstration purposes
    void printInventory() {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << "Item at index " << i << " is at position " << inventory[i] << " (Access count: " << accessCounts[i] << ")\n";
        }
    }

    // Function to get access count for a specific index
    int getAccessCount(size_t index) {
        std::lock_guard<std::mutex> lock(mtx);
        return accessCounts[index];
    }

private:
    std::vector<int> inventory;  // Shared inventory array
    std::vector<std::atomic<int>> accessCounts; // Atomic counters for each inventory section
    std::mutex mtx;              // Mutex for protecting access to the inventory
};

// (Rest of the code remains the same)

// Function to simulate a robot updating an item position
void robotUpdate(Warehouse &warehouse, size_t index, int newPosition) {
    warehouse.updateItemPosition(index, newPosition);
}

// Function to simulate a robot swapping two item positions
void robotSwap(Warehouse &warehouse, size_t index1, size_t index2) {
    warehouse.swapItems(index1, index2);
}

int main() {
    Warehouse warehouse(10); // Create a warehouse with 10 inventory slots

    // Initializing inventory for demonstration
    for (int i = 0; i < 10; ++i) {
        warehouse.updateItemPosition(i, i * 10);  // Positions: 0, 10, 20, ..., 90
    }

    // Suppose multiple robots update inventory positions at the same time
    std::thread robot1(robotUpdate, std::ref(warehouse), 3, 15);
    std::thread robot2(robotUpdate, std::ref(warehouse), 5, 20);
    std::thread robot3(robotSwap, std::ref(warehouse), 3, 5); // This will swap items at index 3 and 5

    // Wait for the robots to finish their updates
    robot1.join();
    robot2.join();
    robot3.join();

    // Print final inventory state
    warehouse.printInventory();

    return 0;
}