#include <iostream>
#include <string>

// Template struct to define loan fee percentages
template<double HousePercentage, double CarPercentage>
struct LoanFees {
    static constexpr double houseLoanFee = HousePercentage;
    static constexpr double carLoanFee = CarPercentage;
};

// Use default values for the LoanFees template instance
using DefaultLoanFees = LoanFees<0.15, 0.10>;

using namespace std;

int main() {
    string loanType;
    double loanAmount;
    double fee;

    // Prompt the user for the type of loan
    cout << "Enter the loan type (house/car): ";
    cin >> loanType;

    // Prompt the user for the loan amount
    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    // Calculate the fee based on the loan type
    if (loanType == "house") {
        fee = DefaultLoanFees::houseLoanFee * loanAmount; // Use the house loan fee
    } else if (loanType == "car") {
        fee = DefaultLoanFees::carLoanFee * loanAmount; // Use the car loan fee
    } else {
        cout << "Invalid loan type entered." << endl;
        return 1; // Exit with an error code
    }

    // Output the calculated fee
    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0; // Successful execution
}