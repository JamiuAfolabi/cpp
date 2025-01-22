#include <iostream>
#include <iomanip>
#include <string>
#include <time.h>

template <typename T>
std::string to_string_with_precision(const T& num, int precision) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << num;
    return ss.str();
}

template <typename T>
void debug_print_transaction(int transaction_id, T amount, const std::string& timestamp) {
    static const int precision = 2;

    std::cout << "Transaction ID: " << transaction_id << std::endl;
    std::cout << "Amount: " << to_string_with_precision(amount, precision) << std::endl;
    std::cout << "Timestamp: " << timestamp << std::endl << std::endl;
}

using MyDecimal = double;

int main() {
    time_t now = time(0);
    std::string timestamp = std::ctime(&now);

    debug_print_transaction(101, 100, timestamp);
    debug_print_transaction(102, 3.14, timestamp);
    debug_print_transaction(103, static_cast<MyDecimal>(123.456), timestamp);

    return 0;
}