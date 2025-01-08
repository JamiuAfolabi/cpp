#include <iostream>
#include <string>

class LoanService {
public:
    // Enum to define the types of loans
    enum LoanType {
        CAR_LOAN,
        HOUSE_LOAN
    };

    // Function to calculate loan origination fee based on loan type and amount
    double calculateLoanFee(LoanType loanType, double loanAmount) {
        switch (loanType) {
            case CAR_LOAN:
                return loanAmount * 0.015; // 1.5% for car loans
            case HOUSE_LOAN:
                return loanAmount * 0.03; // 3% for house loans
            default:
                return 0.0; // Invalid loan type
        }
    }
};

int main() {
    LoanService loanService;

    // Test cases
    double carLoanAmount = 20000.0; // Example car loan amount
    double houseLoanAmount = 300000.0; // Example house loan amount

    double carLoanFee = loanService.calculateLoanFee(LoanService::CAR_LOAN, carLoanAmount);
    double houseLoanFee = loanService.calculateLoanFee(LoanService::HOUSE_LOAN, houseLoanAmount);

    // Output results
    std::cout << "Car Loan Amount: $" << carLoanAmount << std::endl;
    std::cout << "Car Loan Origination Fee: $" << carLoanFee << std::endl;

    std::cout << "House Loan Amount: $" << houseLoanAmount << std::endl;
    std::cout << "House Loan Origination Fee: $" << houseLoanFee << std::endl;

    return 0;
}