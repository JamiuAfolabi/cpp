#include <iostream>
#include <type_traits>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <fstream> // Include this for file handling


// Define mutexes for thread-safe logging and memory tracking
std::mutex log_mutex;

class MemoryStats {
private:
    static std::unordered_map<std::type_index, size_t> totalAllocations;
    static std::unordered_map<std::type_index, size_t> currentAllocations;
    static std::unordered_map<std::type_index, size_t> peakMemoryUsage;
    static std::unordered_map<std::type_index, size_t> totalBytesAllocated;
    static std::mutex statsMutex;

public:
    static void recordAllocation(const std::type_info& type, size_t size) {
        std::lock_guard<std::mutex> lock(statsMutex);
        std::type_index typeIdx(type);
        totalAllocations[typeIdx]++;
        currentAllocations[typeIdx] += size;
        totalBytesAllocated[typeIdx] += size;
        peakMemoryUsage[typeIdx] = std::max(peakMemoryUsage[typeIdx], currentAllocations[typeIdx]);
        
        // Print allocation information
        std::cout << "\nMemory Allocation:\n"
                 << "Type: " << type.name() << "\n"
                 << "Size: " << size << " bytes\n"
                 << "Current total for type: " << currentAllocations[typeIdx] << " bytes\n";
    }

    static void recordDeallocation(const std::type_info& type, size_t size) {
        std::lock_guard<std::mutex> lock(statsMutex);
        std::type_index typeIdx(type);
        currentAllocations[typeIdx] -= size;
    }

    // Updated function to write stats to a file
    static void printStats(const std::string& filename = "memory_stats.txt") {
        std::lock_guard<std::mutex> lock(statsMutex);
        std::ofstream outFile(filename, std::ios::out | std::ios::trunc);

        if (!outFile.is_open()) {
            std::cerr << "Failed to open the file for writing: " << filename << std::endl;
            return;
        }

        outFile << "\n=== Detailed Memory Usage Statistics ===\n";
        for (const auto& pair : totalAllocations) {
            outFile << "\nType: " << pair.first.name() << "\n"
                    << "----------------------------------------\n"
                    << "  Total number of allocations: " << pair.second << "\n"
                    << "  Total bytes allocated: " << totalBytesAllocated[pair.first] << " bytes\n"
                    << "  Current memory usage: " << currentAllocations[pair.first] << " bytes\n"
                    << "  Peak memory usage: " << peakMemoryUsage[pair.first] << " bytes\n"
                    << "  Average allocation size: " 
                    << (pair.second > 0 ? totalBytesAllocated[pair.first] / pair.second : 0) 
                    << " bytes\n";
        }

        outFile << "\n=== Overall Memory Statistics ===\n";
        size_t totalMem = 0, peakMem = 0, currentMem = 0;
        for (const auto& pair : currentAllocations) {
            totalMem += totalBytesAllocated[pair.first];
            peakMem += peakMemoryUsage[pair.first];
            currentMem += pair.second;
        }
        outFile << "Total memory allocated: " << totalMem << " bytes\n"
                << "Peak memory usage: " << peakMem << " bytes\n"
                << "Current memory usage: " << currentMem << " bytes\n\n";

        outFile.close(); // Close the file after writing
        std::cout << "Memory usage statistics have been saved to " << filename << std::endl;
    }
};

// Initialize static members
std::unordered_map<std::type_index, size_t> MemoryStats::totalAllocations;
std::unordered_map<std::type_index, size_t> MemoryStats::currentAllocations;
std::unordered_map<std::type_index, size_t> MemoryStats::peakMemoryUsage;
std::unordered_map<std::type_index, size_t> MemoryStats::totalBytesAllocated;
std::mutex MemoryStats::statsMutex;

// Custom allocator that tracks memory usage
template<typename T>
class TrackingAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = TrackingAllocator<U>;
    };

    TrackingAllocator() noexcept = default;
    
    template<typename U>
    TrackingAllocator(const TrackingAllocator<U>&) noexcept {}

    pointer allocate(size_type n) {
        size_type size = n * sizeof(T);
        pointer p = static_cast<pointer>(::operator new(size));
        MemoryStats::recordAllocation(typeid(T), size);
        return p;
    }

    void deallocate(pointer p, size_type n) noexcept {
        MemoryStats::recordDeallocation(typeid(T), n * sizeof(T));
        ::operator delete(p);
    }
};

// Memory leak detector
class LeakDetector {
private:
    static std::unordered_map<void*, std::string> allocations;
    static std::mutex leakMutex;

public:
    static void recordAllocation(void* ptr, const std::string& type) {
        std::lock_guard<std::mutex> lock(leakMutex);
        allocations[ptr] = type;
    }

    static void recordDeallocation(void* ptr) {
        std::lock_guard<std::mutex> lock(leakMutex);
        allocations.erase(ptr);
    }

    static void checkLeaks() {
        std::lock_guard<std::mutex> lock(leakMutex);
        if (!allocations.empty()) {
            std::cout << "\nPotential memory leaks detected:\n";
            for (const auto& pair : allocations) {
                std::cout << "Address: " << pair.first 
                         << ", Type: " << pair.second << "\n";
            }
        } else {
            std::cout << "\nNo memory leaks detected.\n";
        }
    }
};

// Initialize static members for LeakDetector
std::unordered_map<void*, std::string> LeakDetector::allocations;
std::mutex LeakDetector::leakMutex;

// Transaction struct with tracking allocator
template<typename T>
struct Transaction {
    std::basic_string<char, std::char_traits<char>, TrackingAllocator<char>> transactionId;
    T amount;
    std::basic_string<char, std::char_traits<char>, TrackingAllocator<char>> timestamp;
    
    Transaction(const std::string& id, T amt, const std::string& time)
        : transactionId(id.begin(), id.end()), 
          amount(amt), 
          timestamp(time.begin(), time.end()) {}
};

// Custom Decimal type
class CustomDecimal {
public:
    double value;
    CustomDecimal(double v) : value(v) {}
    friend std::ostream& operator<<(std::ostream& os, const CustomDecimal& cd) {
        os << std::fixed << std::setprecision(2) << cd.value;
        return os;
    }
};

// Template function to print transaction details
template<typename T>
void debug_print(const Transaction<T>& transaction) {
    static_assert(std::is_arithmetic<T>::value || std::is_same<T, std::string>::value,
                  "Amount must be an arithmetic type or a string.");
    std::lock_guard<std::mutex> guard(log_mutex);
    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount << '\n'
              << "Timestamp: " << transaction.timestamp << '\n' << std::endl;
}

// Template specialization for CustomDecimal
template<>
void debug_print(const Transaction<CustomDecimal>& transaction) {
    std::lock_guard<std::mutex> guard(log_mutex);
    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << transaction.amount << '\n'
              << "Timestamp: " << transaction.timestamp << '\n' << std::endl;
}

// Process transaction with memory tracking
template<typename T>
void process_transaction(const std::string& id, T amount, const std::string& timestamp) {
    Transaction<T>* transaction = new Transaction<T>(id, amount, timestamp);
    LeakDetector::recordAllocation(transaction, typeid(T).name());
    
    // Simulate some processing delay
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Process the transaction
    debug_print(*transaction);
    
    // Cleanup
    LeakDetector::recordDeallocation(transaction);
    delete transaction;
}

int main() {
    std::vector<std::thread> threads;
    
    // Create and launch multiple threads to process transactions concurrently
    threads.emplace_back(process_transaction<int>, "TX1001", 250, "2023-01-01 10:00:00");
    threads.emplace_back(process_transaction<double>, "TX1002", 99.99, "2023-01-02 11:00:00");
    threads.emplace_back(process_transaction<std::string>, "TX1003", "100.50", "2023-01-03 12:00:00");
    threads.emplace_back(process_transaction<CustomDecimal>, "TX1004", CustomDecimal(123.45), "2023-01-04 13:00:00");

    // Wait for all threads to finish
    for (std::thread& t : threads) {
        t.join();
    }

    // Print memory statistics and check for leaks
    MemoryStats::printStats();
    LeakDetector::checkLeaks();
    
    return 0;
}