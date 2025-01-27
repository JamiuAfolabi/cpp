#include <iostream>
#include <cstdio>

int main() {
    // Initial account balance
    double balance = 1000.0;
    double withdrawalAmount;

    printf("Welcome to the ATM!\n");
    printf("Your current balance is: $%.2f\n", balance);

    // Asking for withdrawal amount
    printf("Enter the amount you want to withdraw: $");
    scanf("%lf", &withdrawalAmount);

    // Error handling for insufficient funds
    if (withdrawalAmount > balance) {
        printf("Error: Insufficient funds! Your current balance is $%.2f\n", balance);
    } else {
        // Processing the withdrawal
        balance -= withdrawalAmount;
        printf("Withdrawal amount: $%.2f\n", withdrawalAmount);
        printf("Your final balance is: $%.2f\n", balance);
    }

    return 0;
}