#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Warehouse {
public:
    Warehouse(size_t size) : inventory(size) {}

    // Function to safely swap inventory positions
    void swapItemPositions(size_t index1, size_t index2) {
        {
            std::unique_lock<std::mutex> lock1(mtx1, std::try_to_lock);
            std::unique_lock<std::mutex> lock2(mtx2, std::try_to_lock);
            
            // Check if both locks were acquired successfully
            if (lock1.owns_lock() && lock2.owns_lock()) {
                // Swap the elements if indices are valid
                if (index1 < inventory.size() && index2 < inventory.size()) {
                    std::swap(inventory[index1], inventory[index2]);
                    std::cout << "Swapped items at indices " << index1 << " and " << index2 << "\n";
                } else {
                    std::cout << "One or both indices are out of bounds!\n";
                }
            } else {
                std::cout << "Failed to acquire locks to swap items at indices " 
                    << index1 << " and " << index2 << "\n";
            }
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
    std::mutex mtx;              // Mutex for protecting access to inventory during printing
    std::mutex mtx1;              // Mutex for protecting index1
    std::mutex mtx2;              // Mutex for protecting index2
};

// Function to simulate a robot swapping item positions
void robotSwap(Warehouse &warehouse, size_t index1, size_t index2) {
    warehouse.swapItemPositions(index1, index2);
}

int main() {
    Warehouse warehouse(10); // Create a warehouse with 10 inventory slots

    std::thread robot1(robotSwap, std::ref(warehouse), 1, 3);
    std::thread robot2(robotSwap, std::ref(warehouse), 5, 2);
    std::thread robot3(robotSwap, std::ref(warehouse), 3, 7);

    robot1.join();
    robot2.join();
    robot3.join();

    warehouse.printInventory();

    return 0;
} 