#include <iostream>
#include <type_traits>
#include <iomanip>
#include <string>

// Define a struct for the transaction details
template<typename T>
struct Transaction {
    std::string transactionId;
    T amount;
    std::string timestamp;

    Transaction(const std::string& id, T amt, const std::string& time)
        : transactionId(id), amount(amt), timestamp(time) {}
};

// Template function to print transaction details
template<typename T>
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

// Template specialization for CustomDecimal
template<>
void debug_print(const Transaction<CustomDecimal>& transaction) {
    std::cout << "Transaction ID: " << transaction.transactionId << '\n'
              << "Amount: " << transaction.amount << '\n' // CustomDecimal already provides operator<<
              << "Timestamp: " << transaction.timestamp << '\n';
}

int main() {
    // Test with different types
    Transaction<int> t1("TX1001", 250, "2023-01-01 10:00:00");
    Transaction<double> t2("TX1002", 99.99, "2023-01-02 11:00:00");
    Transaction<std::string> t3("TX1003", "100.50", "2023-01-03 12:00:00"); // Note: string amount
    Transaction<CustomDecimal> t4("TX1004", CustomDecimal(123.45), "2023-01-04 13:00:00");

    debug_print(t1);
    std::cout << std::endl;
    debug_print(t2);
    std::cout << std::endl;
    debug_print(t3);
    std::cout << std::endl;
    debug_print(t4);

    return 0;
}