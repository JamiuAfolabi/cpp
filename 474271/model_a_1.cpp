#include <iostream>

enum AccountType {
    SAVINGS,
    FIXED
};

double calculateTax(AccountType accountType, double balance) {
    switch (accountType) {
        case SAVINGS:
            return balance * 0.02;  // 2% tax for savings accounts
        case FIXED:
            return balance * 0.15;   // 15% tax for fixed accounts
        default:
            return 0.0;  // No tax for unknown account types
    }
}

int main() {
    // Define some sample balances for different account types
    double savingsBalance = 1000.0;  // Example balance for a savings account
    double fixedBalance = 2000.0;     // Example balance for a fixed account

    // Calculate taxes
    double savingsTax = calculateTax(SAVINGS, savingsBalance);
    double fixedTax = calculateTax(FIXED, fixedBalance);

    // Output the results
    std::cout << "Savings Account Tax: $" << savingsTax << std::endl;
    std::cout << "Fixed Account Tax: $" << fixedTax << std::endl;

    return 0;
}