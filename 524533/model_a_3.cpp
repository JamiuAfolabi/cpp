#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class Warehouse {
public:
    Warehouse(size_t size) 
        : inventory(size), accessCounters(size) {}

    // Function to safely update the inventory position
    void updateItemPosition(size_t index, int newPosition) {
        if (index < inventory.size()) {
            accessCounters[index].fetch_add(1, std::memory_order_relaxed); // Increment counter
            std::lock_guard<std::mutex> lock(mtx);
            inventory[index] = newPosition;
            std::cout << "Item at index " << index << " updated to position " << newPosition << "\n";
            accessCounters[index].fetch_sub(1, std::memory_order_relaxed); // Decrement counter
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
        
        // Check bounds and perform swap
        if (index1 < inventory.size() && index2 < inventory.size()) {
            // Locking both indices to swap
            accessCounters[index1].fetch_add(1, std::memory_order_relaxed);
            accessCounters[index2].fetch_add(1, std::memory_order_relaxed);
            std::swap(inventory[index1], inventory[index2]);
            std::cout << "Swapped items at index " << index1 << " and " << index2 << "\n";
            accessCounters[index1].fetch_sub(1, std::memory_order_relaxed);
            accessCounters[index2].fetch_sub(1, std::memory_order_relaxed);
        } else {
            std::cout << "Swap failed: one of the indices " << index1 << " or " << index2 << " is out of bounds!\n";
        }
    }

    void printInventory() {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << "Item at index " << i << " is at position " << inventory[i] << ", accessed by " << accessCounters[i].load() << " robots\n";
        }
    }

private:
    std::vector<int> inventory;               // Shared inventory array
    std::vector<std::atomic<int>> accessCounters; // Atomic counters for tracking access
    std::mutex mtx;                           // Mutex for protecting access to the inventory
};

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

    // Simulating multiple robots accessing the inventory
    std::thread robot1(robotUpdate, std::ref(warehouse), 3, 15);
    std::thread robot2(robotUpdate, std::ref(warehouse), 5, 20);
    std::thread robot3(robotSwap, std::ref(warehouse), 3, 5); // Swap items at index 3 and 5

    // Wait for the robots to finish their updates
    robot1.join();
    robot2.join();
    robot3.join();

    // Print final inventory state
    warehouse.printInventory();

    return 0;
}