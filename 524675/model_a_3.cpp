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

// Define a mutex for thread-safe logging
std::mutex log_mutex;

// Custom allocator class
template <typename T>
class TrackingAllocator {
public:
    using value_type = T;

    TrackingAllocator() = default;

    template <typename U>
    TrackingAllocator(const TrackingAllocator<U>&) {}

    T* allocate(std::size_t n) {
        T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));
        if (ptr) {
            std::lock_guard<std::mutex> guard(memory_mutex);
            allocations[sizeof(T)] += n; // Track memory usage by type size
            total_allocated += n;
        }
        return ptr;
    }

    void deallocate(T* ptr, std::size_t n) {
        std::lock_guard<std::mutex> guard(memory_mutex);
        allocations[sizeof(T)] -= n; // Track memory usage by type size
        total_allocated -= n;
        ::operator delete(ptr);
    }

    static void print_memory_usage() {
        std::lock_guard<std::mutex> guard(memory_mutex);
        std::cout << "Memory Usage Statistics:\n";
        for (const auto& entry : allocations) {
            std::cout << "Size: " << entry.first << " bytes, Allocated: " << entry.second << " objects\n";
        }
        std::cout << "Total Allocated Objects: " << total_allocated << "\n\n";
    }

private:
    static std::mutex memory_mutex;
    static std::unordered_map<size_t, size_t> allocations; // Map of size to count
    static size_t total_allocated; // Total allocated objects
};

// Static members initialization
template <typename T>
std::mutex TrackingAllocator<T>::memory_mutex;

template <typename T>
std::unordered_map<size_t, size_t> TrackingAllocator<T>::allocations;

template <typename T>
size_t TrackingAllocator<T>::total_allocated = 0;

// Define a struct for the transaction details using the custom allocator
template<typename T>
struct Transaction {
    std::string transactionId;
    T amount;
    std::string timestamp;

    Transaction(const std::string& id, T amt, const std::string& time)
        : transactionId(id), amount(amt), timestamp(time) {}

    // Overloading new and delete operators for custom allocator support
    void* operator new(std::size_t size) {
        return TrackingAllocator<Transaction>::allocate(1);
    }

    void operator delete(void* ptr) noexcept {
        TrackingAllocator<Transaction>::deallocate(static_cast<Transaction*>(ptr), 1);
    }
};

// Template function to print transaction details
template<typename T>
void debug_print(const Transaction<T>& transaction) {
    static_assert(std::is_arithmetic<T>::value || std::is_same<T, std::string>::value,
                  "Amount must be an arithmetic type or a string.");

    std::lock_guard<std::mutex> guard(log_mutex); // Lock the mutex for thread safety
    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount << '\n'
              << "Timestamp: " << transaction.timestamp << '\n' << std::endl;
}

// Simulated function to process transactions
template<typename T>
void process_transaction(const std::string& id, T amount, const std::string& timestamp) {
    Transaction<T> transaction(id, amount, timestamp);
    // Simulate some processing delay
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    debug_print(transaction);
}

int main() {
    std::vector<std::thread> threads;

    // Create and launch multiple threads to process transactions concurrently
    threads.emplace_back(process_transaction<int>, "TX1001", 250, "2023-01-01 10:00:00");
    threads.emplace_back(process_transaction<double>, "TX1002", 99.99, "2023-01-02 11:00:00");
    threads.emplace_back(process_transaction<std::string>, "TX1003", "100.50", "2023-01-03 12:00:00");

    // Wait for all threads to finish
    for (std::thread& t : threads) {
        t.join();
    }

    // Print memory usage statistics
    TrackingAllocator<Transaction<int>>::print_memory_usage();
    TrackingAllocator<Transaction<double>>::print_memory_usage();
    TrackingAllocator<Transaction<std::string>>::print_memory_usage();

    return 0;
}