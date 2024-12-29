#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

inline double calculateMonthlyPayment(double principal, double annualRate, int years) {
    double monthlyRate = annualRate / 100.0 / 12.0; // Convert annual rate to monthly
    int numPayments = years * 12; // Total number of payments
    return (principal * monthlyRate) / (1 - pow(1 + monthlyRate, -numPayments));
}

inline void generatePaymentSchedule(double principal, double annualRate, int years) {
    double monthlyPayment = calculateMonthlyPayment(principal, annualRate, years);
    double monthlyRate = annualRate / 100.0 / 12.0;
    int numPayments = years * 12;

    double balance = principal;
    cout << fixed << setprecision(2);
    cout << "Payment Schedule:\n";
    cout << "---------------------------------------\n";
    cout << "Payment # | Payment | Interest | Principal | Balance\n";
    cout << "---------------------------------------\n";

    for (int i = 1; i <= numPayments; ++i) {
        double interest = balance * monthlyRate;
        double principalPayment = monthlyPayment - interest;
        balance -= principalPayment;

        cout << setw(9) << i 
             << " | " << setw(7) << monthlyPayment 
             << " | " << setw(8) << interest 
             << " | " << setw(9) << principalPayment 
             << " | " << setw(7) << (balance < 0 ? 0 : balance) 
             << endl;
    }

    cout << "---------------------------------------\n";
}

int main() {
    double principal;
    double annualRate;
    int years;

    cout << "Enter loan principal amount: ";
    cin >> principal;

    cout << "Enter annual interest rate (as a percentage): ";
    cin >> annualRate;

    cout << "Enter loan term in years: ";
    cin >> years;

    generatePaymentSchedule(principal, annualRate, years);

    return 0;
}