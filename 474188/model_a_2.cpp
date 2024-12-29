#include <iostream>
#include <limits>  // For std::numeric_limits
#include <cstdio>  // For printf
using namespace std;

// Function to determine IFRS 9 status
string determineIFRS9Status(double totalExpectedPayments, double totalPaymentsMade) {
    if (totalPaymentsMade < 0) {
        return "Invalid payment amount"; // Invalid case
    }
    
    if (totalPaymentsMade == 0) {
        return "Default"; // No payments made
    }

    double paymentRatio = totalPaymentsMade / totalExpectedPayments;

    if (paymentRatio < 0.5) {
        return "High Risk / Default"; // Less than 50% of payments made
    } else if (paymentRatio >= 0.5 && paymentRatio < 1.0) {
        return "Performing"; // Payments are being made but less than expected
    } else {
        return "Performing / Fully Paid"; // Full payment made
    }
}

// Function to get valid double input from user
double getValidInput(const char* prompt) {
    double input;
    while (true) {
        printf("%s", prompt);
        cin >> input;

        if (cin.fail()) {
            // Handle invalid input (not a number entered)
            cout << "Invalid input. Please enter a numeric value." << endl;
            cin.clear(); // clear input error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard input
        } else if (input <= 0) {
            // Check if number is valid (greater than zero)
            cout << "Input must be greater than zero." << endl;
        } else {
            return input; // valid input
        }
    }
}

int main() {
    // Get valid total expected payments
    double totalExpectedPayments = getValidInput("Enter the total expected payments: ");

    // Get valid total payments made; this can be zero
    double totalPaymentsMade;
    while (true) {
        printf("Enter the total payments made: ");
        cin >> totalPaymentsMade;

        if (cin.fail()) {
            // Handle invalid input (not a number entered)
            cout << "Invalid input. Please enter a numeric value." << endl;
            cin.clear(); // clear input error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard input
        } else {
            break; // valid input acquired
        }
    }

    // Determine and print the IFRS 9 status
    string status = determineIFRS9Status(totalExpectedPayments, totalPaymentsMade);
    printf("The IFRS 9 status is: %s\n", status.c_str());

    return 0;
}