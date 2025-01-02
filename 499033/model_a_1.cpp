#include <iostream>
#include <string>

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
        fee = 0.15 * loanAmount; // 15% fee for house loan
    } else if (loanType == "car") {
        fee = 0.10 * loanAmount; // 10% fee for car loan
    } else {
        cout << "Invalid loan type entered." << endl;
        return 1; // Exit with an error code
    }

    // Output the calculated fee
    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0; // Successful execution
}