#include <iostream>
#include <cmath>
#include <string>

class LoanService {
public:
    enum LoanType {
        CAR_LOAN,
        HOUSE_LOAN
    };

    using FeeCalculationFunc = double (*)(LoanService&, double loanAmount);
    FeeCalculationFunc feeCalculationFuncs[2] = {calculateCarLoanFee, calculateHouseLoanFee};

    // Loan fee calculation functions
    double calculateCarLoanFee(double loanAmount) {
        return loanAmount * 0.015; // 1.5% for car loans
    }

    double calculateHouseLoanFee(double loanAmount) {
        return loanAmount * 0.03; // 3% for house loans
    }

    // Rest of the class code remains the same...
    // Function to calculate monthly payment
    double calculateMonthlyPayment(double principal, double annualInterestRate, int loanTermMonths) {
        double monthlyInterestRate = annualInterestRate / 12 / 100; // Convert annual rate to monthly and percentage
        if (monthlyInterestRate == 0) {
            return principal / loanTermMonths; // Simple division if no interest
        }
        return (principal * monthlyInterestRate) / (1 - pow(1 + monthlyInterestRate, -loanTermMonths));
    }
};
int main() {
    LoanService loanService;

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

    LoanService::LoanType selectedLoanType = (loanType == 1) ? LoanService::CAR_LOAN : LoanService::HOUSE_LOAN;

    // Loan fee calculation
    double loanFee = loanService.calculateLoanFee(selectedLoanType, loanAmount);
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

    return 0;
}