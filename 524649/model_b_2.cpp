#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>

int main() {
    // Initial account balance
    double balance = 1000.0;
    double withdrawalAmount;

    const char* logFilePath = "atm_transactions.log"; // Name of the log file

    printf("Welcome to the ATM!\n");
    printf("Your current balance is: $%.2f\n", balance);

    // Asking for withdrawal amount
    printf("Enter the amount you want to withdraw: $");
    scanf("%lf", &withdrawalAmount);

    // Create a timestamp for logging
    time_t t = time(0);
    struct tm * now = localtime( & t );
    char timestamp[100];
    strftime(timestamp, 100, "%Y-%m-%d %H:%M:%S", now);

    std::ofstream logFile(logFilePath, std::ios_base::app); // Append to the log file

    if (logFile.is_open()) {

        // Log the transaction with timestamp
        logFile << timestamp << " ";

        // Error handling for insufficient funds
        if (withdrawalAmount > balance) {
            logFile << "Failed Withdrawal: Insufficient funds. Current balance: $" << balance << std::endl;
            printf("Error: Insufficient funds! Your current balance is $%.2f\n", balance);
        } else {
            // Processing the withdrawal
            balance -= withdrawalAmount;
            logFile << "Successful Withdrawal: $" << withdrawalAmount << ". Final balance: $" << balance << std::endl;
            printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
            printf("Your final balance is: $%.2f\n", balance);
        }
        logFile.close();
    } 
    else {
        std::cout << "Error opening log file!" << std::endl;
    }

    return 0;
}
