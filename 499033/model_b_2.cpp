#include <iostream>
#include <string>

using namespace std;

// Define loan fee percentages as global constants using #define
#define HOUSE_LOAN_FEE_PERCENTAGE 0.15
#define CAR_LOAN_FEE_PERCENTAGE   0.10

int main() {
    string loanType;
    double loanAmount, fee;

    cout << "Enter the loan type (house/car): ";
    cin >> loanType;

    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    if (loanType == "house") {
        fee = loanAmount * HOUSE_LOAN_FEE_PERCENTAGE;
    } 
    else if (loanType == "car") {
        fee = loanAmount * CAR_LOAN_FEE_PERCENTAGE;
    } 
    else {
        cout << "Invalid loan type entered." << endl;
        return 1;
    }

    cout << "The loan fee for your " << loanType << " loan is: $" << fee << endl;

    return 0;
}