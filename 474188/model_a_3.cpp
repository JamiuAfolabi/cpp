#include <iostream>
#include <limits>  // For std::numeric_limits
#include <cstdio>  // For printf

using namespace std;

// Define constants for error messages
#define INVALID_INPUT_MSG "Invalid input. Please enter a numeric value.\n"
#define LESS_THAN_ZERO_MSG "Input must be greater than zero.\n"
#define TOTAL_EXPECTED_PROMPT "Enter the total expected payments: "
#define TOTAL_PAYMENTS_PROMPT "Enter the total payments made: "

// Function to get valid double input from user
double getValidInput(const char* prompt) {
    double input;
    while (true) {
        printf("%s", prompt);
        cin >> input;

        if (cin.fail()) {
            // Handle invalid input (not a number entered)
            printf(INVALID_INPUT_MSG);
            cin.clear(); // clear input error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        } else if (input <= 0) {
            // Check if the number is valid (greater than zero)
            printf(LESS_THAN_ZERO_MSG);
        } else {
            return input; // valid input
        }
    }
}

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

int main() {
    double totalExpectedPayments = getValidInput(TOTAL_EXPECTED_PROMPT);
    double totalPaymentsMade = getValidInput(TOTAL_PAYMENTS_PROMPT);

    // Determine and print the IFRS 9 status
    string status = determineIFRS9Status(totalExpectedPayments, totalPaymentsMade);
    printf("The IFRS 9 status is: %s\n", status.c_str());

    return 0;
}