#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

struct LogEntry {
    std::string message;
    std::chrono::system_clock::time_point timestamp;

    LogEntry(const std::string& msg) : message(msg), timestamp(std::chrono::system_clock::now()) {}
};

class Logger {
public:
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        logEntries.emplace_back(message);
    }

    void printLogs() {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& entry : logEntries) {
            std::time_t time = std::chrono::system_clock::to_time_t(entry.timestamp);
            std::cout << "[" << std::ctime(&time) << "] " << entry.message << std::endl;
        }
    }

private:
    std::vector<LogEntry> logEntries;
    std::mutex mtx;
};

class Warehouse {
public:
    Warehouse(size_t size) : inventory(size), accessCounts(size) {
        for (auto& count : accessCounts) {
            count.store(0);
        }
    }

    void updateItemPosition(size_t index, int newPosition) {
        if (index >= inventory.size()) {
            std::cout << "Index out of bounds!\n";
            return;
        }

        accessCounts[index].fetch_add(1, std::memory_order_seq_cst);
        
        // Log the start of access
        logger.log("Robot started access on index " + std::to_string(index) + " (Access count: " + std::to_string(accessCounts[index].load()) + ")");
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            inventory[index] = newPosition;
            logger.log("Updated index " + std::to_string(index) + " to position " + std::to_string(newPosition));
        }
        
        accessCounts[index].fetch_sub(1, std::memory_order_seq_cst);
    }

    void swapItems(size_t index1, size_t index2) {
        if (index1 >= inventory.size() || index2 >= inventory.size()) {
            std::cout << "Swap failed: Index out of bounds!\n";
            return;
        }

        accessCounts[index1].fetch_add(1, std::memory_order_seq_cst);
        accessCounts[index2].fetch_add(1, std::memory_order_seq_cst);
        
        // Log the start of swap
        logger.log("Swap operation starting on indices " + std::to_string(index1) + " and " + std::to_string(index2) +
                   " (Access counts: " + std::to_string(accessCounts[index1].load()) + ", " + std::to_string(accessCounts[index2].load()) + ")");
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::swap(inventory[index1], inventory[index2]);
            logger.log("Swapped items at index " + std::to_string(index1) + " and " + std::to_string(index2));
        }
        
        accessCounts[index1].fetch_sub(1, std::memory_order_seq_cst);
        accessCounts[index2].fetch_sub(1, std::memory_order_seq_cst);
    }

    void printInventory() {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << "Index " << i << ": position " << inventory[i] 
                      << " (Access count: " << accessCounts[i].load() << ")\n";
        }
    }

    Logger& getLogger() { return logger; }

private:
    std::vector<int> inventory;
    std::vector<std::atomic<int>> accessCounts;
    Logger logger; // Logger instance
    std::mutex mtx; // Mutex for inventory access
};

void robotUpdate(Warehouse &warehouse, size_t index, int newPosition) {
    warehouse.updateItemPosition(index, newPosition);
}

void robotSwap(Warehouse &warehouse, size_t index1, size_t index2) {
    warehouse.swapItems(index1, index2);
}

int main() {
    Warehouse warehouse(10);

    // Initializing inventory for demonstration
    for (int i = 0; i < 10; ++i) {
        warehouse.updateItemPosition(i, i * 10);
    }

    std::cout << "\nStarting concurrent operations:\n";

    std::thread robot1(robotUpdate, std::ref(warehouse), 3, 15);
    std::thread robot2(robotUpdate, std::ref(warehouse), 5, 20);
    std::thread robot3(robotSwap, std::ref(warehouse), 3, 5);

    robot1.join();
    robot2.join();
    robot3.join();

    std::cout << "\nFinal state:\n";
    warehouse.printInventory();

    std::cout << "\nLog Entries:\n";
    warehouse.getLogger().printLogs();

    return 0;
}