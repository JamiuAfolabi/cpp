#include <iostream>
#include <string>

// Define loan fee percentages as global constants
#define HOUSE_LOAN_FEE_PERCENTAGE 0.15
#define CAR_LOAN_FEE_PERCENTAGE 0.10

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
        fee = HOUSE_LOAN_FEE_PERCENTAGE * loanAmount; // Use the defined constant
    } else if (loanType == "car") {
        fee = CAR_LOAN_FEE_PERCENTAGE * loanAmount; // Use the defined constant
    } else {
        cout << "Invalid loan type entered." << endl;
        return 1; // Exit with an error code
    }

    // Output the calculated fee
    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0; // Successful execution
}