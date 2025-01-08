#include <iostream>
#include <cmath>
#include <functional>

template <typename LoanType>
class LoanService {
public:
    // Type definition for loan fee calculation function
    using LoanFeeCalcFunc = std::function<double(double)>;

    LoanService() {
        // Initialize the function pointer array with appropriate functions
        loanFeeCalculators[0] = [this](double loanAmount) { return loanAmount * LoanType::CAR_LOAN_FEE_RATE; };
        loanFeeCalculators[1] = [this](double loanAmount) { return loanAmount * LoanType::HOUSE_LOAN_FEE_RATE; };
    }

    // Function pointer array for loan fee calculation
    LoanFeeCalcFunc loanFeeCalculators[2];  // For simplicity, we assume two types of loans here

    // Function to calculate loan origination fee based on loan type and amount
    double calculateLoanFee(int loanType, double loanAmount) {
        if (loanType < 0 || loanType >= 2) {
            return 0.0; // Invalid loan type
        }
        return loanFeeCalculators[loanType](loanAmount);
    }

    // Function to calculate monthly payment
    double calculateMonthlyPayment(double principal, double annualInterestRate, int loanTermMonths) {
        if (loanTermMonths == 0) return 0.0; // Avoid division by zero
        double monthlyInterestRate = annualInterestRate / 12 / 100; // Convert annual rate to monthly and percentage
        if (monthlyInterestRate == 0) {
            return principal / loanTermMonths; // Simple division if no interest
        }
        return (principal * monthlyInterestRate) / (1 - pow(1 + monthlyInterestRate, -loanTermMonths));
    }
};

// Loan Type Definitions using Template Specialization
struct CarLoan {
    static constexpr double CAR_LOAN_FEE_RATE = 0.015;  // 1.5% for car loans
    static constexpr double HOUSE_LOAN_FEE_RATE = 0.03; // 3% for house loans (won't be used for this type)
};

struct HouseLoan {
    static constexpr double CAR_LOAN_FEE_RATE = 0.0;   // Not applicable for house loans
    static constexpr double HOUSE_LOAN_FEE_RATE = 0.03; // 3% for house loans
};

int main() {
    double loanAmount, annualInterestRate;
    int loanTermMonths;
    int feeOption;

    // Prompt the user for loan details
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

    // Create LoanService object based on selected loan type
    if (loanType == 1) {
        LoanService<CarLoan> loanService;  // For car loans
        // Loan fee calculation
        double loanFee = loanService.calculateLoanFee(0, loanAmount);
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
        std::cout << "Monthly Payment: $" << monthlyPayment << std::endl;
    }
    else if (loanType == 2) {
        LoanService<HouseLoan> loanService;  // For house loans
        // Loan fee calculation
        double loanFee = loanService.calculateLoanFee(1, loanAmount);
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
        std::cout << "Monthly Payment: $" << monthlyPayment << std::endl;
    }

    return 0;
}
