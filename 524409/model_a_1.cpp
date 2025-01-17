#include <iostream>
#include <iomanip>
#include <vector>

class Loan {
private:
    double loanAmount;
    double annualInterestRate;
    int loanDuration;          // in years
    int paymentFrequency;      // number of payments per year

public:
    Loan(double amount, double rate, int duration, int frequency)
        : loanAmount(amount), annualInterestRate(rate), loanDuration(duration), paymentFrequency(frequency) {}

    double calculateMonthlyInterestRate() {
        return (annualInterestRate / 100) / paymentFrequency;
    }

    int totalPayments() {
        return loanDuration * paymentFrequency;
    }

    double calculatePeriodicPayment() {
        double interestRate = calculateMonthlyInterestRate();
        int totalPayments = this->totalPayments();
        
        // Formula for calculating the monthly/periodic payment (PMT)
        double payment = (loanAmount * interestRate) / (1 - pow((1 + interestRate), -totalPayments));
        return payment;
    }

    void generateAmortizationSchedule() {
        double periodicPayment = calculatePeriodicPayment();
        double remainingBalance = loanAmount;
        double totalInterestPaid = 0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Payment\t\tPrincipal\tInterest\tBalance\n";

        for (int i = 1; i <= totalPayments(); ++i) {
            double interest = remainingBalance * calculateMonthlyInterestRate();
            double principal = periodicPayment - interest;
            remainingBalance -= principal;
            totalInterestPaid += interest;

            std::cout << i << "\t\t" 
                      << principal << "\t\t" 
                      << interest << "\t\t" 
                      << remainingBalance << "\n";
        }

        std::cout << "Total payments: " << totalPayments() << "\n";
        std::cout << "Total interest paid: $" << totalInterestPaid << "\n";
        std::cout << "Total payment: $" << (totalPayments() * periodicPayment) << "\n";
    }
};

int main() {
    double loanAmount;
    double annualInterestRate;
    int loanDuration;
    int paymentFrequency;

    std::cout << "Enter loan amount: ";
    std::cin >> loanAmount;

    std::cout << "Enter annual interest rate (in %): ";
    std::cin >> annualInterestRate;

    std::cout << "Enter loan duration (in years): ";
    std::cin >> loanDuration;

    std::cout << "Enter payment frequency (1 for Annually, 12 for Monthly): ";
    std::cin >> paymentFrequency;

    Loan myLoan(loanAmount, annualInterestRate, loanDuration, paymentFrequency);
    myLoan.generateAmortizationSchedule();

    return 0;
}