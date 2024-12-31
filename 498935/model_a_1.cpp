#include <iostream>
#include <iomanip>

using namespace std;

// Function prototypes
double calculateLongTermFee(double loanAmount);
double calculateShortTermFee(double loanAmount);

// Typedef for function pointer
typedef double (*FeeCalculator)(double);

int main() {
    // Array of function pointers
    FeeCalculator feeCalculators[2];
    feeCalculators[0] = calculateLongTermFee;
    feeCalculators[1] = calculateShortTermFee;

    // User input
    double loanAmount;
    int loanType;

    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    cout << "Select loan type:\n";
    cout << "1. Long-term loan (10% fee)\n";
    cout << "2. Short-term loan (20% fee)\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> loanType;

    // Validate loanType input
    if (loanType < 1 || loanType > 2) {
        cout << "Invalid loan type selected. Please choose 1 or 2." << endl;
        return 1;
    }

    // Call the appropriate function using the function pointer
    double fee = feeCalculators[loanType - 1](loanAmount);
    
    // Display the result
    cout << fixed << setprecision(2);
    cout << "The loan fee is: $" << fee << endl;

    return 0;
}

// Function to calculate long-term loan fee
double calculateLongTermFee(double loanAmount) {
    return loanAmount * 0.10; // 10% fee
}

// Function to calculate short-term loan fee
double calculateShortTermFee(double loanAmount) {
    return loanAmount * 0.20; // 20% fee
}