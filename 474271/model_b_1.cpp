#include<iostream>
using namespace std;

enum class AccountType {
    SAVINGS,
    FIXED
};

double calculateTax(AccountType accountType, double interestEarned)
{
    double tax = 0.0;
    switch (accountType)
    {
        case AccountType::SAVINGS:
            tax = interestEarned * 0.02;
            break;
        case AccountType::FIXED:
            tax = interestEarned * 0.15;
            break;
    }
    return tax;
}

int main() {

    AccountType accountType = AccountType::SAVINGS; // or AccountType::FIXED
    double interestEarned = 100.0; // Replace this with your desired interest amount

    double taxAmount = calculateTax(accountType, interestEarned);

    cout << "Account Type: ";
    switch (accountType)
    {
        case AccountType::SAVINGS:
            cout << "Savings" << endl;
            break;
        case AccountType::FIXED:
            cout << "Fixed" << endl;
            break;
    }

    cout << "Interest Earned: " << interestEarned << endl;
    cout << "Tax Calculated: " << taxAmount << endl;

    return 0;
}