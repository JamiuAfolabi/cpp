#include <iostream>
#include <iomanip>
#include <functional>

using namespace std;

// Typedef for function pointer
typedef double (*FeeCalculator)(double);

int main() {
    // User input for loan amount
    double loanAmount;
    cout << "Enter the loan amount: $";
    cin >> loanAmount;

    // Option for user to provide custom fee calculation logic
    cout << "\nDo you want to use a custom fee calculation logic (y/n)? ";
    char choice;
    cin >> choice;

    FeeCalculator feeCalculator = nullptr;

    // Default fee calculation functions
    FeeCalculator defaultCalculators[2] = {
        &calculateLongTermFee,  // Long-term fee: 10%
        &calculateShortTermFee  // Short-term fee: 20%
    };

    if (choice == 'y') {
        // User provides their custom function pointer
        cout << "Enter the name of your custom fee calculation function: ";
        string functionName;
        cin >> functionName;

        if (functionName == "custom_calculate_fee") {
            // Hardcoded example of a custom function
            feeCalculator = &custom_calculate_fee;
        } else {
            cout << "Invalid custom function name. Using default short-term fee." << endl;
            feeCalculator = defaultCalculators[1];
        }
    } else if (choice == 'n') {
        cout << "Select loan type:\n";
        cout << "1. Long-term loan (10% fee)\n";
        cout << "2. Short-term loan (20% fee)\n";
        int loanType;
        cout << "Enter your choice (1 or 2): ";
        cin >> loanType;

        if (loanType < 1 || loanType > 2) {
            cout << "Invalid loan type selected. Using default short-term fee." << endl;
        }
        feeCalculator = defaultCalculators[loanType - 1];
    } else {
        cout << "Invalid input. Using default short-term fee." << endl;
        feeCalculator = defaultCalculators[1];
    }

    if (!feeCalculator) {
        cout << "Invalid fee calculation function selected." << endl;
        return 1;
    }

    // Call the selected function (user-provided or default)
    double fee = feeCalculator(loanAmount);
    
    // Display the result
    cout << fixed << setprecision(2);
    cout << "The loan fee is: $" << fee << endl;

    return 0;
}

// Function to calculate long-term loan fee (10%)
double calculateLongTermFee(double loanAmount) {
    return loanAmount * 0.1;
}

// Function to calculate short-term loan fee (20%)
double calculateShortTermFee(double loanAmount) {
    return loanAmount * 0.2;
}

// Example of a custom function
double custom_calculate_fee(double loanAmount) {
    // Your custom logic here. For example, fee = min(loanAmount * 0.15, 100.0)
    return min(loanAmount * 0.15, 100.0);
}
