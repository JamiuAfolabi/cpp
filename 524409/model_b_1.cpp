#include <iostream>
#include <vector>
using namespace std;

// Struct to represent Loan details
struct Loan {
    double loanAmount;
    double annualInterestRate;
    int loanDuration;  // years
    string paymentFrequency; // "Monthly" or "Bi-Weekly"

    Loan(double amount, double rate, int duration, const string& frequency)
        : loanAmount(amount), annualInterestRate(rate), loanDuration(duration), paymentFrequency(frequency) {}
};

// Function to calculate monthly interest rate
double calculateMonthlyInterestRate(double annualInterestRate) {
    return annualInterestRate / 1200;
}

// Function to calculate bi-weekly interest rate
double calculateBiWeeklyInterestRate(double annualInterestRate) {
    return annualInterestRate / 2600;
}

// Function to calculate number of payments based on loan duration and frequency
int calculateNumberOfPayments(int loanDuration, const string& frequency) {
    if (frequency == "Monthly") {
        return loanDuration * 12;
    } else if (frequency == "Bi-Weekly") {
        return loanDuration * 26;
    } else {
        cerr << "Invalid payment frequency. Valid options are 'Monthly' and 'Bi-Weekly'.\n";
        return -1;
    }
}

// Function to calculate the periodic payment
double calculatePeriodicPayment(Loan loan) {
    int numPayments = calculateNumberOfPayments(loan.loanDuration, loan.paymentFrequency);
    
    if (numPayments == -1) {
        return -1.0;
    }

    double interestRate;
    if (loan.paymentFrequency == "Monthly") {
        interestRate = calculateMonthlyInterestRate(loan.annualInterestRate);
    } else if (loan.paymentFrequency == "Bi-Weekly") {
        interestRate = calculateBiWeeklyInterestRate(loan.annualInterestRate);
    }
    double monthlyPayment = loan.loanAmount * interestRate * pow(1 + interestRate, numPayments) 
                            / (pow(1 + interestRate, numPayments) - 1);

    return monthlyPayment;
}

// Function to generate the amortization schedule
vector<string> generateAmortizationSchedule(Loan loan) {
    int numPayments = calculateNumberOfPayments(loan.loanDuration, loan.paymentFrequency);
    
    if (numPayments == -1) {
        return vector<string>();
    }

    double interestRate, payment = calculatePeriodicPayment(loan);
    double principalPaid, interestPaid, remainingBalance = loan.loanAmount;
    vector<string> schedule;

    for (int i = 1; i <= numPayments; ++i) {
        interestRate = loan.paymentFrequency == "Monthly" ? 
                        calculateMonthlyInterestRate(loan.annualInterestRate) : 
                        calculateBiWeeklyInterestRate(loan.annualInterestRate);
        interestPaid = remainingBalance * interestRate;
        principalPaid = payment - interestPaid;
        remainingBalance -= principalPaid;

        double round_principal = static_cast<double>(round(100 * principalPaid) / 100);
        double round_interest = static_cast<double>(round(100 * interestPaid) / 100);
        double round_remaining = static_cast<double>(round(100 * remainingBalance) / 100);

        stringstream line;
        line << "Payment " << i << ": Principal Paid: $" << round_principal 
             << ", Interest Paid: $" << round_interest
             << ", Remaining Balance: $" << round_remaining << endl;
        schedule.push_back(line.str());
    }

    return schedule;
}

// Main function to drive the loan management system
int main() {
    Loan loan;
    cout << "Loan Management System:" << endl;

    cout << "Enter the loan amount: $";
    cin >> loan.loanAmount;
    if (!cin.good()) {
        cerr << "Invalid input for loan amount." << endl;
        return 1;
    }

    cout << "Enter the annual interest rate (e.g., 5 for 5%): ";
    cin >> loan.annualInterestRate;
    if (!cin.good()) {
        cerr << "Invalid input for interest rate." << endl;
        return 1;
    }

    cout << "Enter the loan duration in years: ";
    cin >> loan.loanDuration;
    if (!cin.good() || loan.loanDuration <= 0) {
        cerr << "Invalid input for loan duration. Must be a positive integer." << endl;
        return 1;
    }

    cout << "Enter the payment frequency (Monthly/Bi-Weekly): ";
    string frequency;
    getline(cin, frequency);
    transform(frequency.begin(), frequency.end(), frequency.begin(), ::toupper);

    if (frequency != "MONTHLY" && frequency != "BI-WEEKLY") {
        cerr << "Invalid payment frequency. Valid options are 'Monthly' and 'Bi-Weekly'." << endl;
        return 1;
    }

    loan.paymentFrequency = frequency;

    double payment = calculatePeriodicPayment(loan);
    if (payment == -1.0) {
        return 1;
    }

    cout << "Periodic Payment: $" << payment << endl;
    
    vector<string> amortizationSchedule = generateAmortizationSchedule(loan);
    cout << "Amortization Schedule:" << endl;
    for (const string& line : amortizationSchedule) {
        cout << line;
    }

    return 0;
}