#include <iostream>
#include <iomanip>
using namespace std;

// Function to calculate the monthly payment
double calculateMonthlyPayment(double loanAmount, double annualInterestRate, int loanTermInYears) {
    double monthlyInterestRate = annualInterestRate / 1200;
    int totalPayments = loanTermInYears * 12;

    double payment = loanAmount * monthlyInterestRate * pow(1 + monthlyInterestRate, totalPayments) /
                     (pow(1 + monthlyInterestRate, totalPayments) - 1);

    return payment;
}

// Function to generate the loan payment schedule
void generatePaymentSchedule(double loanAmount, double monthlyPayment, int loanTermInYears) {
    int totalPayments = loanTermInYears * 12;

    cout << "\nLoan Payment Schedule:\n";
    cout << "=======================\n";
    cout << "Month" << setw(10) << "Payment" << setw(10) << "Interest" << setw(10) << "Principal" << setw(10) << "Balance" << endl;
    cout << "-----" << setw(10) << "--------" << setw(10) << "--------" << setw(10) << "--------" << endl;

    double balance = loanAmount;
    for (int month = 1; month <= totalPayments; month++) {
        double interest = balance * (monthlyInterestRate * 100);
        double principal = monthlyPayment - interest;
        balance -= principal;

        cout << month << setw(10) << fixed << setprecision(2) << monthlyPayment << setw(10)
             << fixed << setprecision(2) << interest << setw(10) << fixed << setprecision(2)
             << principal << setw(10) << fixed << setprecision(2) << balance << endl;
    }
}

int main() {
    double loanAmount, annualInterestRate, monthlyPayment;
    int loanTermInYears;

    cout << "Enter Loan Amount: $";
    cin >> loanAmount;

    cout << "Enter Annual Interest Rate (e.g., 5 for 5%): ";
    cin >> annualInterestRate;

    cout << "Enter Loan Term in years: ";
    cin >> loanTermInYears;

    monthlyPayment = calculateMonthlyPayment(loanAmount, annualInterestRate, loanTermInYears);

    cout << "\nLoan Details:\n";
    cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << endl;
    cout << "Annual Interest Rate: " << annualInterestRate << "%" << endl;
    cout << "Loan Term: " << loanTermInYears << " years" << endl;
    cout << "Monthly Payment: $" << fixed << setprecision(2) << monthlyPayment << endl;

    generatePaymentSchedule(loanAmount, monthlyPayment, loanTermInYears);

    return 0;
}