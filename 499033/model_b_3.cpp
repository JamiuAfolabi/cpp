#include <iostream>
#include <string>

// Default loan fee percentages
#ifndef HOUSE_LOAN_FEE_PERCENTAGE
#define HOUSE_LOAN_FEE_PERCENTAGE 0.15
#endif

#ifndef CAR_LOAN_FEE_PERCENTAGE
#define CAR_LOAN_FEE_PERCENTAGE 0.10
#endif

using namespace std;

int main() {
    string loanType;
    double loanAmount, fee;

    // Prompt the user for the type of loan
    cout << "Enter the loan type (house/car): ";
    cin >> loanType;

    // Prompt the user for the loan amount
    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    // Calculate the fee based on the loan type
    if (loanType == "house") {
        fee = HOUSE_LOAN_FEE_PERCENTAGE * loanAmount;
    } else if (loanType == "car") {
        fee = CAR_LOAN_FEE_PERCENTAGE * loanAmount;
    } else {
        cout << "Invalid loan type entered." << endl;
        return 1;
    }

    // Output the calculated fee
    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0;
}
