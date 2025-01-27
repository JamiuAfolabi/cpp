#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <iomanip>

void logTransaction(const std::string& transactionType, double withdrawalAmount, double previousBalance, double newBalance, const std::string& errorMessage = "") {
    std::ofstream logfile("transaction_log.txt", std::ios_base::app); // Append mode
    if (logfile.is_open()) {
        // Get the current time
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        // Format the timestamp
        logfile << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << ", "
                << transactionType << ", "
                << (errorMessage.empty() ? std::to_string(withdrawalAmount) : errorMessage) << ", "
                << previousBalance << ", "
                << (errorMessage.empty() ? std::to_string(newBalance) : "N/A") << "\n";
        logfile.close();
    } else {
        std::cerr << "Unable to open log file!" << std::endl;
    }
}

int main() {
    // Initial account balance
    double balance = 1000.0;
    double withdrawalAmount;

    printf("Welcome to the ATM!\n");
    printf("Your current balance is: $%.2f\n", balance);

    // Transaction loop
    while (true) {
        printf("Enter the amount you want to withdraw (or enter 0 to exit): $");
        scanf("%lf", &withdrawalAmount);

        // Exit condition
        if (withdrawalAmount == 0) {
            printf("Exiting the ATM. Thank you!\n");
            break;
        }

        // Store previous balance for logging
        double previousBalance = balance;

        // Error handling for insufficient funds
        if (withdrawalAmount > balance) {
            printf("Error: Insufficient funds! Your current balance is $%.2f\n", balance);
            logTransaction("Error", withdrawalAmount, previousBalance, balance, "Insufficient funds");
        } else {
            // Processing the withdrawal
            balance -= withdrawalAmount;
            printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
            printf("Your final balance is: $%.2f\n", balance);
            logTransaction("Withdrawal", withdrawalAmount, previousBalance, balance);
        }
    }

    return 0;
}