#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#define NUM_ROBOTS 3
#define NUM_ITEMS 10

std::mutex inventory_mutex;
std::vector<int> inventory_positions(NUM_ITEMS, -1);

void updateInventoryPosition(int robot_id, int item_id, int new_position) {
    {
        std::lock_guard<std::mutex> lock(inventory_mutex);

        if (item_id >= 0 && item_id < NUM_ITEMS) {
            std::cout << "Robot " << robot_id << " updating position of item " << item_id << " to " << new_position << std::endl;
            inventory_positions[item_id] = new_position;
        } else {
            std::cout << "Error: Item ID " << item_id << " is out of range." << std::endl;
        }
    }
}

void simulateRobotWork(int robot_id) {
    for (int i = 0; i < 5; i++) {
        int item_id = std::rand() % NUM_ITEMS;
        int new_position = std::rand() % 100;
        updateInventoryPosition(robot_id, item_id, new_position);
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 200));
    }
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    std::thread robots[NUM_ROBOTS];

    for (int i = 0; i < NUM_ROBOTS; i++) {
        robots[i] = std::thread(simulateRobotWork, i);
    }

    for (int i = 0; i < NUM_ROBOTS; i++) {
        robots[i].join();
    }

    std::cout << "\nFinal Inventory Positions:\n";
    for (int i = 0; i < NUM_ITEMS; i++) {
        std::cout << "Item " << i << ": Position " << inventory_positions[i] << std::endl;
    }

    return 0;
}