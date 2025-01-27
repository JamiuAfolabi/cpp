#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

const std::size_t MAX_LOG_SIZE = 1024 * 1024; // 1 MB

struct Transaction {
    std::string timestamp;
    std::string transactionType;
    double amount;
    double balanceBefore;
    double balanceAfter;
    bool success;
    std::string errorMessage;
};

std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void logTransaction(const Transaction& trans, std::ofstream& logFile) {
    logFile << trans.timestamp << ","
            << trans.transactionType << ","
            << trans.amount << ","
            << trans.balanceBefore << ","
            << trans.balanceAfter << ","
            << (trans.success ? "true" : "false") << ","
            << "\"" << trans.errorMessage << "\"\n";
    logFile.flush();
}

void rotateLogFile(const std::string& logFilePath) {
    if (fs::exists(logFilePath) && fs::file_size(logFilePath) >= MAX_LOG_SIZE) {
        std::string newFileName = logFilePath + "_" + getCurrentTimestamp() + ".log";
        fs::rename(logFilePath, newFileName);
        std::ofstream(logFilePath, std::ios::app);
    }
}

int countSuccessfulWithdrawals(const std::string& logFilePath) {
    int successfulWithdrawals = 0;
    std::ifstream logFile(logFilePath);

    if (logFile.is_open()) {
        std::string line;
        std::getline(logFile, line); // Skip the header

        while (std::getline(logFile, line)) {
            std::istringstream iss(line);
            std::string success;
            getline(iss, success, ',');
            if (success == "true") {
                successfulWithdrawals++;
            }
        }
        logFile.close();
    } else {
        std::cout << "Error: Cannot open log file for analysis." << std::endl;
    }

    return successfulWithdrawals;
}

int main() {
    double balance = 1000.0;
    double withdrawalAmount;
    std::string logFilePath = "transaction_log.csv";
    std::ofstream logFile(logFilePath, std::ios::app);

    logFile.seekp(0, std::ios::end);
    if (logFile.tellp() == 0) {
        logFile << "Timestamp,TransactionType,Amount,BalanceBefore,BalanceAfter,Success,ErrorMessage\n";
    }

    printf("Welcome to the ATM!\n");
    printf("Your current balance is: $%.2f\n", balance);

    int successfulWithdrawals = 0;

    while (true) {
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

        if (withdrawalAmount < 0) {
            trans.success = false;
            trans.errorMessage = "Invalid amount requested";
            trans.balanceAfter = balance;
        } else if (withdrawalAmount > balance) {
            trans.success = false;
            trans.errorMessage = "Insufficient funds";
            trans.balanceAfter = balance;
        } else {
            balance -= withdrawalAmount;
            trans.success = true;
            trans.errorMessage = "";
            trans.balanceAfter = balance;
            successfulWithdrawals++;
        }

        logTransaction(trans, logFile);

        printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
        printf("Your final balance is: $%.2f\n", balance);

        rotateLogFile(logFilePath);
    }

    printf("Thank you for using our ATM!\n");
    printf("Total successful withdrawal attempts: %d\n", successfulWithdrawals);
    printf("Detailed transaction logs are available in the 'transaction_log.csv' file and its rotated versions.\n");

    logFile.close();

    return 0;
}
