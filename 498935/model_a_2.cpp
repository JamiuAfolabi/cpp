#include <iostream>
#include <iomanip>
#include <functional>

using namespace std;

// Default fee calculation functions
double defaultLongTermCalc(double loanAmount) {
    return loanAmount * 0.10; // 10% fee
}

double defaultShortTermCalc(double loanAmount) {
    return loanAmount * 0.20; // 20% fee
}

// Typedef for a function pointer to the fee calculation function
typedef double (*FeeCalculator)(double);

// Function to accept the user's custom calculation
FeeCalculator getCustomCalculation() {
    // In a real-world scenario, you'd probably want this function
    // to be derived from user input or compile-time definitions.
    return [](double loanAmount) {
        // Example of a custom calculation logic
        return loanAmount * 0.15; // 15% fee as an example
    };
}

int main() {
    // Default function pointers
    FeeCalculator feeCalculators[2];
    feeCalculators[0] = defaultLongTermCalc;
    feeCalculators[1] = defaultShortTermCalc;

    // User input for custom callback function
    char useCustomCalculation;
    cout << "Would you like to use a custom loan fee calculation? (y/n): ";
    cin >> useCustomCalculation;

    FeeCalculator customCalculator = nullptr;
    if (useCustomCalculation == 'y' || useCustomCalculation == 'Y') {
        customCalculator = getCustomCalculation();
    }

    // User input
    double loanAmount;
    int loanType;

    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    cout << "Select loan type:\n";
    cout << "1. Long-term loan\n";
    cout << "2. Short-term loan\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> loanType;

    // Validate loanType input
    if (loanType < 1 || loanType > 2) {
        cout << "Invalid loan type selected. Please choose 1 or 2." << endl;
        return 1;
    }

    // Use user-defined callback function if available
    FeeCalculator selectedCalculator = customCalculator ? customCalculator : feeCalculators[loanType - 1];
    
    // Call the appropriate function using the function pointer
    double fee = selectedCalculator(loanAmount);
    
    // Display the result
    cout << fixed << setprecision(2);
    cout << "The loan fee is: $" << fee << endl;

    return 0;
}