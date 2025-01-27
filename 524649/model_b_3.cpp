#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>
namespace fs = std::filesystem;

// Global constant for log file size limit (in KB)
const int LOG_FILE_SIZE_LIMIT = 100;

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

std::string getCurrentLogFilePath() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S_") << "atm_log.csv";
    return oss.str();
}

void logTransaction(const Transaction& trans) {
    std::string logFilePath = getCurrentLogFilePath();

    // Check if the file exists and if it's size exceeds the limit
    if (fs::exists(logFilePath)) {
        std::ifstream logFile(logFilePath);
        logFile.seekg(0, std::ios::end);
        int fileSize = logFile.tellg() / 1024;  // Size in KB
        logFile.close();

        if (fileSize > LOG_FILE_SIZE_LIMIT) {
            // Rotate log file - Create a new log file
            logFilePath = getCurrentTimestamp() + "_" + logFilePath;
        }
    }

    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        printf("Error: Unable to open log file!\n");
        return;
    }

    // ... (Same logging logic as before)
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