#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

// Structure to hold transaction details
struct Transaction {
    std::string timestamp;
    std::string transactionType;
    double amount;
    double balanceBefore;
    double balanceAfter;
    bool success;
    std::string errorMessage;
};

// Function to get current timestamp
std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Function to log transaction to file
void logTransaction(const Transaction& trans) {
    std::ofstream logFile("transaction_log.csv", std::ios::app);
    if (!logFile.is_open()) {
        printf("Error: Unable to open log file!\n");
        return;
    }
    
    // Write CSV header if file is empty
    logFile.seekp(0, std::ios::end);
    if (logFile.tellp() == 0) {
        logFile << "Timestamp,TransactionType,Amount,BalanceBefore,BalanceAfter,Success,ErrorMessage\n";
    }
    
    // Log transaction in CSV format
    logFile << trans.timestamp << ","
            << trans.transactionType << ","
            << trans.amount << ","
            << trans.balanceBefore << ","
            << trans.balanceAfter << ","
            << (trans.success ? "true" : "false") << ","
            << "\"" << trans.errorMessage << "\"\n";
    
    logFile.close();
}

int main() {
    // Initial account balance
    double balance = 1000.0;
    double withdrawalAmount;
    
    printf("Welcome to the ATM!\n");
    printf("Your current balance is: $%.2f\n", balance);
    
    while (true) {
        // Transaction object to store current operation
        Transaction trans;
        trans.timestamp = getCurrentTimestamp();
        trans.transactionType = "WITHDRAWAL";
        trans.balanceBefore = balance;
        
        printf("\nEnter the amount you want to withdraw (0 to exit): $");
        scanf("%lf", &withdrawalAmount);
        
        if (withdrawalAmount == 0) {
            break;
        }
        
        trans.amount = withdrawalAmount;
        
        // Error handling for invalid amount
        if (withdrawalAmount < 0) {
            trans.success = false;
            trans.errorMessage = "Invalid amount requested";
            trans.balanceAfter = balance;
            logTransaction(trans);
            printf("Error: Invalid amount entered!\n");
            continue;
        }
        
        // Error handling for insufficient funds
        if (withdrawalAmount > balance) {
            trans.success = false;
            trans.errorMessage = "Insufficient funds";
            trans.balanceAfter = balance;
            logTransaction(trans);
            printf("Error: Insufficient funds! Your current balance is $%.2f\n", balance);
            continue;
        }
        
        // Processing the withdrawal
        balance -= withdrawalAmount;
        trans.success = true;
        trans.errorMessage = "";
        trans.balanceAfter = balance;
        
        logTransaction(trans);
        
        printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
        printf("Your final balance is: $%.2f\n", balance);
    }
    
    printf("Thank you for using our ATM!\n");
    return 0;
}