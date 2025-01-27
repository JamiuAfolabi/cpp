#include <iostream>
#include <cstdio>

class ATM {
private:
    double balance;

public:
    ATM(double initial_balance) : balance(initial_balance) {}

    void withdraw(double amount) {
        if (amount > balance) {
            printf("Error: Insufficient funds. Current balance: $%.2f\n", balance);
        } else {
            balance -= amount;
            printf("Withdrawal Amount: $%.2f\n", amount);
            printf("Current Balance: $%.2f\n", balance);
        }
    }

    void displayBalance() {
        printf("Current Balance: $%.2f\n", balance);
    }
};

int main() {
    double initial_balance;
    double withdrawal_amount;
    
    // Initialize account with an initial balance
    printf("Enter initial balance: $");
    scanf("%lf", &initial_balance);
    
    ATM atm(initial_balance);
    atm.displayBalance();

    while (true) {
        printf("Enter withdrawal amount (or 0 to exit): $");
        scanf("%lf", &withdrawal_amount);
        
        if (withdrawal_amount == 0) {
            printf("Exiting ATM. Final Balance: $%.2f\n", atm.getBalance());
            break;
        }

        atm.withdraw(withdrawal_amount);
    }

    return 0;
}