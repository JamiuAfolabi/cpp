#include <iostream>
#include <type_traits>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <unordered_map>
#include <new>
#include <algorithm>

// Mutex for thread-safe memory usage tracking
std::mutex memory_mutex;

// Custom allocator to track memory usage
template <typename T>
class MemoryTrackerAllocator : public std::allocator<T> {
public:
    using value_type = typename std::allocator<T>::value_type;

    MemoryTrackerAllocator() noexcept = default;
    MemoryTrackerAllocator(const MemoryTrackerAllocator&) noexcept = default;
    template <typename U>
    MemoryTrackerAllocator(const MemoryTrackerAllocator<U>&) noexcept {}

    ~MemoryTrackerAllocator() {
    }

    value_type* allocate(size_t n, const std::allocator<void>::const_pointer& hint = nullptr) noexcept {
        value_type* ptr = std::allocator<T>::allocate(n, hint);
        std::lock_guard<std::mutex> guard(memory_mutex);
        total_allocated += n * sizeof(value_type);
        return ptr;
    }

    void deallocate(value_type* ptr, size_t n) noexcept {
        std::lock_guard<std::mutex> guard(memory_mutex);
        total_allocated -= n * sizeof(value_type);
        std::allocator<T>::deallocate(ptr, n);
    }

    static size_t get_total_allocated() {
        std::lock_guard<std::mutex> guard(memory_mutex);
        return total_allocated;
    }

private:
    static size_t total_allocated;
};

template <typename T>
size_t MemoryTrackerAllocator<T>::total_allocated = 0;

// Define a struct for the transaction details
template<typename T, typename Allocator = MemoryTrackerAllocator<T>>
struct Transaction {
    std::string transactionId;
    T amount;
    std::string timestamp;

    Transaction(const std::string& id, T amt, const std::string& time)
        : transactionId(id), amount(amt), timestamp(time) {}
};

// ... (rest of the code remains the same)

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

// Specialization for custom decimal types, as previously defined
class CustomDecimal {
public:
    double value;

    CustomDecimal(double v) : value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const CustomDecimal& cd) {
        os << std::fixed << std::setprecision(2) << cd.value; // Format to two decimal places
        return os;
    }
};

// Template specialization for CustomDecimal
template<>
void debug_print(const Transaction<CustomDecimal>& transaction) {
    std::lock_guard<std::mutex> guard(log_mutex); // Lock the mutex for thread safety

    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << transaction.amount << '\n'
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
    threads.emplace_back(process_transaction<CustomDecimal>, "TX1004", CustomDecimal(123.45), "2023-01-04 13:00:00");

    // Wait for all threads to finish
    for (std::thread& t : threads) {
        t.join();
    }

    // Print total memory allocated
    std::cout << "Total memory allocated: " << MemoryTrackerAllocator<int>::get_total_allocated() << " bytes\n";

    return 0;
}