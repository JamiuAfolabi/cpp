#include <iostream>
#include <cmath> // For the pow function

using namespace std;

int main() {
    double principal, annualInterestRate, totalAmount, totalInterest;
    int timesCompounded, years;

    // User input
    cout << "Enter the principal amount: ";
    cin >> principal;

    cout << "Enter the annual interest rate (in percentage): ";
    cin >> annualInterestRate;

    cout << "Enter the number of times interest is compounded per year: ";
    cin >> timesCompounded;

    cout << "Enter the number of years: ";
    cin >> years;

    // Convert annual interest rate from percentage to decimal
    double rate = annualInterestRate / 100.0;

    // Calculate the total amount using the compound interest formula
    totalAmount = principal * pow(1 + (rate / timesCompounded), timesCompounded * years);

    // Calculate the total interest accumulated
    totalInterest = totalAmount - principal;

    // Output the results
    cout << "Total amount after " << years << " years: $" << totalAmount << endl;
    cout << "Total interest accumulated: $" << totalInterest << endl;

    return 0;
}