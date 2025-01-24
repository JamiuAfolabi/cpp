#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

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
       std::cout << "Robot starting access on index " << index 
                 << " (Access count: " << accessCounts[index].load() << ")\n";
       
       // Simulate work
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
       
       {
           std::lock_guard<std::mutex> lock(mtx);
           inventory[index] = newPosition;
           std::cout << "Updated index " << index << " to position " << newPosition << "\n";
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
       
       std::cout << "Swap operation starting on indices " << index1 << " and " << index2 
                 << " (Access counts: " << accessCounts[index1].load() 
                 << ", " << accessCounts[index2].load() << ")\n";
       
       // Simulate work
       std::this_thread::sleep_for(std::chrono::milliseconds(150));

       {
           std::lock_guard<std::mutex> lock(mtx);
           std::swap(inventory[index1], inventory[index2]);
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

private:
   std::vector<int> inventory;
   std::vector<std::atomic<int>> accessCounts;
   std::mutex mtx;
};

void robotUpdate(Warehouse &warehouse, size_t index, int newPosition) {
   warehouse.updateItemPosition(index, newPosition);
}

void robotSwap(Warehouse &warehouse, size_t index1, size_t index2) {
   warehouse.swapItems(index1, index2);
}

int main() {
   Warehouse warehouse(10);

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

   return 0;
}