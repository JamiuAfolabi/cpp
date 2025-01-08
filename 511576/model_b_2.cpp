#include <iostream>
#include <string>

class LoanService {
public:
    enum LoanType {
        CAR_LOAN,
        HOUSE_LOAN
    };

    enum FeePaymentOption {
        UPFRONT,
        ROLL_INTO_LOAN
    };

    double calculateLoanFee(LoanType loanType, double loanAmount) {
        switch (loanType) {
            case CAR_LOAN:
                return loanAmount * 0.015;
            case HOUSE_LOAN:
                return loanAmount * 0.03;
            default:
                return 0.0;
        }
    }

    double calculateMonthlyPayment(double loanAmount, double interestRate, int loanTermInMonths, FeePaymentOption feePaymentOption, double loanFee) {
        double interestRatePerMonth = interestRate / 1200;
        double totalLoanAmount = loanAmount;

        if (feePaymentOption == FeePaymentOption::ROLL_INTO_LOAN) {
            totalLoanAmount += loanFee;
        }

        double monthlyPayment = totalLoanAmount * interestRatePerMonth * std::pow(1 + interestRatePerMonth, loanTermInMonths) /
                               (std::pow(1 + interestRatePerMonth, loanTermInMonths) - 1);

        return monthlyPayment;
    }
};

int main() {
    LoanService loanService;
    LoanService::LoanType loanType;
    LoanService::FeePaymentOption feePaymentOption;

    double loanPrincipal, interestRate, loanTermInYears, loanFee;

    std::cout << "Loan Type:\n"
              << "1. Car Loan\n"
              << "2. House Loan\n"
              << "Please choose a loan type (1/2): ";
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 2) {
        std::cout << "Invalid choice. Please select again (1/2): ";
        std::cin >> choice;
    }

    switch (choice) {
        case 1:
            loanType = LoanService::CAR_LOAN;
            break;
        case 2:
            loanType = LoanService::HOUSE_LOAN;
            break;
    }

    std::cout << "Enter loan principal: $";
    std::cin >> loanPrincipal;

    std::cout << "Enter annual interest rate (e.g., 5 for 5%): ";
    std::cin >> interestRate;

    std::cout << "Enter loan term in years: ";
    std::cin >> loanTermInYears;

    std::cout << "Pay loan fee:\n"
              << "1. Upfront\n"
              << "2. Roll it into the loan amount\n"
              << "Please choose an option (1/2): ";
    std::cin >> choice;
    while (choice < 1 || choice > 2) {
        std::cout << "Invalid choice. Please select again (1/2): ";
        std::cin >> choice;
    }

    switch (choice) {
        case 1:
            feePaymentOption = LoanService::FeePaymentOption::UPFRONT;
            break;
        case 2:
            feePaymentOption = LoanService::FeePaymentOption::ROLL_INTO_LOAN;
            break;
    }

    loanFee = loanService.calculateLoanFee(loanType, loanPrincipal);

    std::cout << "Loan Fee: $" << loanFee << std::endl;

    int loanTermInMonths = loanTermInYears * 12;

    double monthlyPayment = loanService.calculateMonthlyPayment(
            loanPrincipal, interestRate, loanTermInMonths, feePaymentOption, loanFee);

    std::cout << "Monthly Payment: $" << monthlyPayment << std::endl;

    return 0;
}