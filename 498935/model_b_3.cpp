#include <iostream>
#include <iomanip>
#include <map>
#include <functional>
#include <string>

using namespace std;

// Typedef for a function pointer to the fee calculation function
typedef double (*FeeCalculator)(double);

// Default fee calculation functions
double defaultLongTermCalc(double loanAmount) {
    return loanAmount * 0.10; // 10% fee
}

double defaultShortTermCalc(double loanAmount) {
    return loanAmount * 0.20; // 20% fee
}

// Function to create a new custom fee calculation function
FeeCalculator createCustomCalculation() {
    // Simulate user input for custom calculation logic
    double customRate;
    cout << "Enter custom fee rate (e.g., 0.15 for 15%): ";
    cin >> customRate;

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
    cin >> loanAmount;

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
    if (loanType < 1 || loanType > 2) {
        cout << "Invalid loan type for new fee structure. Please choose 3 or higher." << endl;
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