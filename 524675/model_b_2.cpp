#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <type_traits>
#include <iomanip>
#include <string>

// Define a struct for the transaction details
template <typename T>
struct Transaction {
    std::string transactionId;
    T amount;
    std::string timestamp;

    Transaction(const std::string& id, T amt, const std::string& time)
        : transactionId(id), amount(amt), timestamp(time) {}
};

// Template function to print transaction details
template <typename T>
void debug_print(const Transaction<T>& transaction) {
    static_assert(std::is_arithmetic<T>::value || std::is_same<T, std::string>::value,
                  "Amount must be an arithmetic type or a string.");

    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << std::fixed << std::setprecision(2) << transaction.amount << '\n'
              << "Timestamp: " << transaction.timestamp << '\n';
}

// Specialization for custom decimal types (if they don't fit into arithmetic)
class CustomDecimal {
public:
    // Assume you have a custom implementation
    // Example data member
    double value;
    
    CustomDecimal(double v) : value(v) {}
    
    // Define output stream operator for custom decimal
    friend std::ostream& operator<<(std::ostream& os, const CustomDecimal& cd) {
        os << std::fixed << std::setprecision(2) << cd.value; // Format to two decimal places
        return os;
    }
};
template <>
void debug_print(const Transaction<CustomDecimal>& transaction) {
    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << transaction.amount << '\n' // CustomDecimal already provides operator<<
              << "Timestamp: " << transaction.timestamp << '\n';
}

// Thread-safe transaction processor
class ThreadSafeTransactionProcessor {
public:
    using TransactionType = Transaction<double>; // Assuming double for amounts in this processor

    void enqueueTransaction(const TransactionType& transaction) {
        std::unique_lock<std::mutex> lock(mutex);
        transactions.push(transaction);
        condition.notify_one();
    }

    void startProcessing() {
        std::thread worker([this] {
            while (true) {
                TransactionType transaction;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this] { return !transactions.empty() || stop; });
                    if (stop) {
                        return;
                    }
                    transaction = std::move(transactions.front());
                    transactions.pop();
                }
                processTransaction(transaction);
            }
        });
        worker.detach();
    }

    void processTransaction(const TransactionType& transaction) {
        // Simulate transaction processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 100));
        debug_print(transaction);
    }

    void stopProcessing() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
    }

private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<TransactionType> transactions;
    bool stop = false;
};

int main() {
    ThreadSafeTransactionProcessor processor;
    processor.startProcessing();

    // Enqueue multiple transactions concurrently
    for (int i = 1; i <= 10; ++i) {
        auto transaction = Transaction<double>("TX" + std::to_string(i), std::rand() % 100.0,
                                                "2023-07-31 " + std::to_string(i) + ":" + std::to_string(i * 10) + ":00");
        processor.enqueueTransaction(transaction);
    }

    // Add some more transactions after a delay
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (int i = 11; i <= 15; ++i) {
        auto transaction = Transaction<double>("TX" + std::to_string(i), std::rand() % 100.0,
                                                "2023-07-31 " + std::to_string(i) + ":" + std::to_string(i * 10) + ":00");
        processor.enqueueTransaction(transaction);
    }

    // Wait for processing to complete
    processor.stopProcessing();
    processor.join();

    return 0;
}