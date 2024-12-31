#include <iostream>
#include <iomanip>
#include <functional>
#include <map>
#include <string>

using namespace std;

// Typedef for the fee calculation function
typedef double (*FeeCalculator)(double);

// Function templates for default calculations
double defaultLongTermCalc(double loanAmount) {
    return loanAmount * 0.10; // 10% fee
}

double defaultShortTermCalc(double loanAmount) {
    return loanAmount * 0.20; // 20% fee
}

// A function to display available fee structures
void displayAvailableCalculations(const map<string, FeeCalculator>& calculations) {
    cout << "Available fee calculations:\n";
    for (const auto& entry : calculations) {
        cout << "- " << entry.first << "\n";
    }
}

// Main program logic
int main() {
    // Map to hold fee calculation functions with their names
    map<string, FeeCalculator> calculations;
    calculations["default long-term"] = defaultLongTermCalc;
    calculations["default short-term"] = defaultShortTermCalc;

    double loanAmount;
    string chosenCalculation;

    while (true) {
        cout << "Enter the loan amount (or 'exit' to quit): ";
        string input;
        cin >> input;

        if (input == "exit") {
            break;
        }

        loanAmount = stod(input);

        // Display available calculations
        displayAvailableCalculations(calculations);
        
        // Choose a calculation
        cout << "Choose a calculation by its name: ";
        cin >> chosenCalculation;

        // Check if the chosen calculation exists
        if (calculations.find(chosenCalculation) == calculations.end()) {
            cout << "Invalid calculation selected. Please try again.\n";
            continue;
        }

        // Calculate the fee
        FeeCalculator selectedCalculator = calculations[chosenCalculation];
        double fee = selectedCalculator(loanAmount);
        
        // Display the result
        cout << fixed << setprecision(2);
        cout << "The loan fee is: $" << fee << endl;

        // Optionally add/update calculations
        char modifyChoice;
        cout << "Would you like to modify the fee calculations? (y/n): ";
        cin >> modifyChoice;

        if (modifyChoice == 'y' || modifyChoice == 'Y') {
            string newCalcName;
            cout << "Enter a name for the new calculation: ";
            cin >> newCalcName;

            // Define a simple custom fee calculation
            calculations[newCalcName] = [](double amount) {
                // Example of a custom calculation logic
                double customRate;
                cout << "Enter the custom fee percentage (as a decimal): ";
                cin >> customRate; // e.g., for 15%, enter 0.15
                return amount * customRate; // Apply custom rate
            };
            cout << "Custom calculation added: " << newCalcName << endl;
        }
    }

    return 0;
}