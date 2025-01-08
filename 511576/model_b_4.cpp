#include <iostream>
#include <cmath>
#include <functional>

template <typename LoanType>
class LoanCalculator {
public:
    LoanCalculator() {
        loanFeeCalculator = [this](double loanAmount) {
            return calculateLoanFee(loanAmount);
        };
    }

    virtual double calculateLoanFee(double loanAmount) = 0;

    std::function<double(double)> loanFeeCalculator;
};

class CarLoanCalculator : public LoanCalculator<CarLoanCalculator> {
public:
    double calculateLoanFee(double loanAmount) override {
        return loanAmount * 0.015; // 1.5% for car loans
    }
};

class HouseLoanCalculator : public LoanCalculator<HouseLoanCalculator> {
public:
    double calculateLoanFee(double loanAmount) override {
        return loanAmount * 0.03; // 3% for house loans
    }
};

class LoanService {
public:
    template <typename LoanCalcType>
    double calculateLoanFee(double loanAmount, LoanCalcType& loanCalc) {
        return loanCalc.loanFeeCalculator(loanAmount);
    }

    template <typename LoanCalcType>
    double calculateMonthlyPayment(double principal, double annualInterestRate, int loanTermMonths, LoanCalcType& loanCalc) {
        double monthlyInterestRate = annualInterestRate / 12 / 100;
        if (monthlyInterestRate == 0) {
            return principal / loanTermMonths;
        }
        return (principal * monthlyInterestRate) / (1 - pow(1 + monthlyInterestRate, -loanTermMonths));
    }
};

int main() {
    LoanService loanService;

    double loanAmount, annualInterestRate;
    int loanTermMonths, feeOption;

    std::cout << "Enter the amount of the loan: ";
    std::cin >> loanAmount;

    std::cout << "Enter the annual interest rate (in %): ";
    std::cin >> annualInterestRate;

    std::cout << "Enter the loan term (in months): ";
    std::cin >> loanTermMonths;

    int loanType;
    std::cout << "Select loan type:\n1. Car Loan\n2. House Loan\nChoose (1/2): ";
    std::cin >> loanType;

    CarLoanCalculator carLoanCalc;
    HouseLoanCalculator houseLoanCalc;

    LoanCalculator<>* loanCalcPtr;

    if (loanType == 1) {
        loanCalcPtr = &carLoanCalc;
    } else {
        loanCalcPtr = &houseLoanCalc;
    }

    double loanFee = loanService.calculateLoanFee(loanAmount, *loanCalcPtr);
    std::cout << "Loan Origination Fee: $" << loanFee << std::endl;

    std::cout << "Do you want to pay the loan fee upfront or roll it into the loan?\n1. Pay upfront\n2. Roll into loan\nChoose (1/2): ";
    std::cin >> feeOption;

    double principal = loanAmount;
    if (feeOption == 2) {
        principal += loanFee;
    }

    double monthlyPayment = loanService.calculateMonthlyPayment(principal, annualInterestRate, loanTermMonths, *loanCalcPtr);
    std::cout << "Monthly Payment: $" << monthlyPayment << std::endl;

    return 0;
}
