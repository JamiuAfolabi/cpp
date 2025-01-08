#include <iostream>
#include <cmath>
#include <functional>
#include <vector>

// Template structure to handle different loan types
template<typename LoanType>
class LoanService {
public:
    // Method to calculate loan origination fee
    double calculateLoanFee(LoanType loanType, double loanAmount) {
        return loanType.calculateFee(loanAmount);
    }

    // Method to calculate monthly payment
    double calculateMonthlyPayment(double principal, double annualInterestRate, int loanTermMonths) {
        if (loanTermMonths == 0) return 0.0; // Avoid division by zero
        double monthlyInterestRate = annualInterestRate / 12 / 100; // Convert annual rate to monthly
        if (monthlyInterestRate == 0) {
            return principal / loanTermMonths; // Simple division if no interest
        }
        return (principal * monthlyInterestRate) / (1 - pow(1 + monthlyInterestRate, -loanTermMonths));
    }
};

// Abstract loan type class
class LoanTypeBase {
public:
    virtual double calculateFee(double loanAmount) const = 0; // Pure virtual function
};

// Car loan type class
class CarLoan : public LoanTypeBase {
public:
    double calculateFee(double loanAmount) const override {
        return loanAmount * 0.015; // 1.5% for car loans
    }
};

// House loan type class
class HouseLoan : public LoanTypeBase {
public:
    double calculateFee(double loanAmount) const override {
        return loanAmount * 0.03; // 3% for house loans
    }
};

int main() {
    LoanService<LoanTypeBase*> loanService;

    double loanAmount, annualInterestRate;
    int loanTermMonths;
    int feeOption;

    std::cout << "Enter the amount of the loan: ";
    std::cin >> loanAmount;

    std::cout << "Enter the annual interest rate (in %): ";
    std::cin >> annualInterestRate;

    std::cout << "Enter the loan term (in months): ";
    std::cin >> loanTermMonths;

    // Loan type selection
    int loanType;
    std::cout << "Select loan type:\n1. Car Loan\n2. House Loan\nChoose (1/2): ";
    std::cin >> loanType;

    LoanTypeBase* selectedLoanType = nullptr;
    if (loanType == 1) {
        selectedLoanType = new CarLoan();
    } else if (loanType == 2) {
        selectedLoanType = new HouseLoan();
    }

    // Loan fee calculation
    double loanFee = loanService.calculateLoanFee(*selectedLoanType, loanAmount);
    std::cout << "Loan Origination Fee: $" << loanFee << std::endl;

    // Fee payment option
    std::cout << "Do you want to pay the loan fee upfront or roll it into the loan?\n1. Pay upfront\n2. Roll into loan\nChoose (1/2): ";
    std::cin >> feeOption;

    double principal = loanAmount;
    if (feeOption == 2) {
        principal += loanFee; // Roll fee into loan amount
    }

    // Calculate monthly payment
    double monthlyPayment = loanService.calculateMonthlyPayment(principal, annualInterestRate, loanTermMonths);

    // Output the results
    std::cout << "Monthly Payment: $" << monthlyPayment << std::endl;

    // Clean up
    delete selectedLoanType;

    return 0;
}