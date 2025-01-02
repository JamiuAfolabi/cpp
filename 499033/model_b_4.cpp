#include <iostream>
using namespace std;

// Compile-time constants defined using templates
template <typename T>
struct LoanFee {
    static constexpr T house = 0.15;
    static constexpr T car = 0.10;
};

int main() {
    string loanType;
    double loanAmount, fee;

    // Prompt the user for the type of loan
    cout << "Enter the loan type (house/car): ";
    cin >> loanType;

    // Prompt the user for the loan amount
    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    // Calculate the fee based on the loan type using the template constants
    if (loanType == "house") {
        fee = LoanFee<double>::house * loanAmount;
    } else if (loanType == "car") {
        fee = LoanFee<double>::car * loanAmount;
    } else {
        cout << "Invalid loan type entered." << endl;
        return 1;
    }

    // Output the calculated fee
    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0;
}