#include <iostream>
#include <iomanip>
#include <map>
#include <functional>
#include <string>
#include <limits>  // for numeric_limits

using namespace std;

// Typedef for a function pointer to the fee calculation function
// We will use std::function instead of a raw function pointer
typedef std::function<double(double)> FeeCalculator;

// Default fee calculation functions
double defaultLongTermCalc(double loanAmount) {
    return loanAmount * 0.10; // 10% fee
}

double defaultShortTermCalc(double loanAmount) {
    return loanAmount * 0.20; // 20% fee
}

// Function to create a new custom fee calculation function
FeeCalculator createCustomCalculation() {
    double customRate;

    // Prompt the user for the custom fee rate and validate input
    cout << "Enter custom fee rate (e.g., 0.15 for 15%): ";

    while (true) {
        cin >> customRate;

        // Check if the input is valid
        if (cin.fail()) {
            cout << "Invalid input. Please enter a valid decimal number: ";
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        } else {
            break; // exit loop if input is valid
        }
    }

    // Use a lambda to create the custom fee calculator
    return [customRate](double loanAmount) {
        return loanAmount * customRate;
    };
}

// Store fee calculators in a map
map<int, FeeCalculator> feeCalculatorsMap = {
    {1, defaultLongTermCalc},
    {2, defaultShortTermCalc}
};

void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Calculate Loan Fee\n";
    cout << "2. Add New Fee Structure\n";
    cout << "3. Update Existing Fee Structure\n";
    cout << "4. Delete Existing Fee Structure\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void calculateLoanFee() {
    double loanAmount;
    int loanType;

    cout << "Enter the loan amount: ";
    
    // Validate loan amount input
    while (true) {
        cin >> loanAmount;

        if (cin.fail()) {
            cout << "Invalid loan amount. Please enter a valid number: ";
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        } else {
            break; // exit loop if input is valid
        }
    }

    cout << "Select loan type:\n";
    cout << "1. Long-term loan\n";
    cout << "2. Short-term loan\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> loanType;

    // Validate loanType input
    if (loanType < 1 || loanType > 2) {
        cout << "Invalid loan type selected. Please choose 1 or 2." << endl;
        return;
    }

    auto feeCalculator = feeCalculatorsMap.find(loanType);
    if (feeCalculator == feeCalculatorsMap.end()) {
        cout << "Fee structure for loan type " << loanType << " not found." << endl;
        return;
    }

    double fee = feeCalculator->second(loanAmount);
    
    // Display the result
    cout << fixed << setprecision(2);
    cout << "The loan fee is: $" << fee << endl;
}

void addNewFeeStructure() {
    int loanType;
    cout << "Enter loan type (1 or 2 to replace existing, or any other value for new): ";
    cin >> loanType;

    // Validate loanType input
    if (loanType < 1) {
        cout << "Invalid loan type for new fee structure. Please choose 1 or higher." << endl;
        return;
    }

    FeeCalculator newCalculator = createCustomCalculation();
    feeCalculatorsMap[loanType] = newCalculator;
    cout << "Fee structure added successfully for loan type " << loanType << endl;
}

void updateExistingFeeStructure() {
    int loanType;
    cout << "Select loan type to update fee structure:\n";
    cout << "1. Long-term loan\n";
    cout << "2. Short-term loan\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> loanType;

    // Validate loanType input
    if (loanType < 1 || loanType > 2) {
        cout << "Invalid loan type selected for update. Please choose 1 or 2." << endl;
        return;
    }

    FeeCalculator newCalculator = createCustomCalculation();
    feeCalculatorsMap[loanType] = newCalculator;
    cout << "Fee structure updated successfully for loan type " << loanType << endl;
}

int main() {
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                calculateLoanFee();
                break;
            case 2:
                addNewFeeStructure();
                break;
            case 3:
                updateExistingFeeStructure();
                break;
            case 4:
                cout << "Delete feature not implemented yet.\n";
                break;
            case 5:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid choice. Please choose a valid option.\n";
        }
    }

    return 0;
}
