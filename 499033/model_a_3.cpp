#include <iostream>
#include <string>

// Template to define loan fee percentages
template<double HOUSE_LOAN_FEE_PERCENTAGE, double CAR_LOAN_FEE_PERCENTAGE>
class LoanCalculator {
public:
    static double calculateFee(const std::string& loanType, double loanAmount) {
        double fee = 0.0;

        if (loanType == "house") {
            fee = HOUSE_LOAN_FEE_PERCENTAGE * loanAmount; // Use the template parameter
        } else if (loanType == "car") {
            fee = CAR_LOAN_FEE_PERCENTAGE * loanAmount; // Use the template parameter
        } else {
            std::cerr << "Invalid loan type entered." << std::endl;
            return -1; // Return -1 to indicate an error
        }

        return fee;
    }
};

int main() {
    // Define the loan fee percentages at compile time
    constexpr double houseFeePercentage = 0.15;
    constexpr double carFeePercentage = 0.10;

    // Create a LoanCalculator instance with specified percentages
    using MyLoanCalculator = LoanCalculator<houseFeePercentage, carFeePercentage>;

    std::string loanType;
    double loanAmount;

    // Prompt the user for the type of loan
    std::cout << "Enter the loan type (house/car): ";
    std::cin >> loanType;

    // Prompt the user for the loan amount
    std::cout << "Enter the loan amount: ";
    std::cin >> loanAmount;

    // Calculate the fee using the LoanCalculator
    double fee = MyLoanCalculator::calculateFee(loanType, loanAmount);
    
    if (fee >= 0) {
        // Output the calculated fee
        std::cout << "The loan fee for your " << loanType << " loan is: $" << fee << std::endl;
    }

    return 0; // Successful execution
}