#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Define the maximum log file size (in bytes)
const std::size_t MAX_LOG_SIZE = 1024 * 1024; // 1 MB

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
void logTransaction(const Transaction& trans, std::ofstream& logFile) {
    // Write transaction in CSV format
    logFile << trans.timestamp << ","
            << trans.transactionType << ","
            << trans.amount << ","
            << trans.balanceBefore << ","
            << trans.balanceAfter << ","
            << (trans.success ? "true" : "false") << ","
            << "\"" << trans.errorMessage << "\"\n";
    logFile.flush(); // Ensure data is written to file
}

// Function to rotate log file if it exceeds the size limit
void rotateLogFile(const std::string& logFilePath) {
    if (fs::exists(logFilePath) && fs::file_size(logFilePath) >= MAX_LOG_SIZE) {
        // Generate a new file name using a timestamp
        std::string newFileName = logFilePath + "_" + getCurrentTimestamp() + ".log";
        fs::rename(logFilePath, newFileName); // Rename the old log file
        std::ofstream(logFilePath, std::ios::app); // Create a new log file
    }
}

// Function to analyze the log file and count successful withdrawals
int countSuccessfulWithdrawals(const std::string& logFilePath) {
    std::ifstream logFile(logFilePath);
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open log file for analysis!" << std::endl;
        return -1;
    }

    std::string line;
    int successfulCount = 0;

    // Skip the header line
    std::getline(logFile, line);

    while (std::getline(logFile, line)) {
        std::string successFlag;
        std::istringstream lineStream(line);
        std::getline(lineStream, line, ','); // Timestamp
        std::getline(lineStream, line, ','); // TransactionType
        std::getline(lineStream, line, ','); // Amount
        std::getline(lineStream, line, ','); // BalanceBefore
        std::getline(lineStream, line, ','); // BalanceAfter
        std::getline(lineStream, successFlag, ','); // Success
        // Skip the error message
        std::getline(lineStream, line); // ErrorMessage

        if (successFlag == "true") {
            successfulCount++;
        }
    }

    logFile.close();
    return successfulCount;
}

int main() {
    // Initial account balance
    double balance = 1000.0;
    double withdrawalAmount;
    
    std::string logFilePath = "transaction_log.csv";
    std::ofstream logFile(logFilePath, std::ios::app);
    
    // Check if log file is created and write header if necessary
    logFile.seekp(0, std::ios::end);
    if (logFile.tellp() == 0) {
        logFile << "Timestamp,TransactionType,Amount,BalanceBefore,BalanceAfter,Success,ErrorMessage\n";
    }

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
            logTransaction(trans, logFile);
            printf("Error: Invalid amount entered!\n");
            continue;
        }
        
        // Error handling for insufficient funds
        if (withdrawalAmount > balance) {
            trans.success = false;
            trans.errorMessage = "Insufficient funds";
            trans.balanceAfter = balance;
            logTransaction(trans, logFile);
            printf("Error: Insufficient funds! Your current balance is $%.2f\n", balance);
            continue;
        }
        
        // Processing the withdrawal
        balance -= withdrawalAmount;
        trans.success = true;
        trans.errorMessage = "";
        trans.balanceAfter = balance;
        
        logTransaction(trans, logFile);
        
        printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
        printf("Your final balance is: $%.2f\n", balance);
        
        // Rotate log file if needed
        rotateLogFile(logFilePath);
    }
    
    // Analyze and count successful withdrawals
    int successfulWithdrawals = countSuccessfulWithdrawals(logFilePath);
    if (successfulWithdrawals >= 0) {
        printf("Total successful withdrawal attempts: %d\n", successfulWithdrawals);
    }

    printf("Thank you for using our ATM!\n");

    logFile.close();
    return 0;
}