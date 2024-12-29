#include <iostream>
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

int main() {
    double totalExpectedPayments, totalPaymentsMade;

    // User input
    cout << "Enter the total expected payments: ";
    cin >> totalExpectedPayments;

    // Validate input
    if (totalExpectedPayments <= 0) {
        cout << "Total expected payments must be greater than zero." << endl;
        return 1;
    }

    cout << "Enter the total payments made: ";
    cin >> totalPaymentsMade;

    // Validate input
    if (totalPaymentsMade < 0) {
        cout << "Total payments made cannot be negative." << endl;
        return 1;
    }

    // Determine and print the IFRS 9 status
    string status = determineIFRS9Status(totalExpectedPayments, totalPaymentsMade);
    cout << "The IFRS 9 status is: " << status << endl;

    return 0;
}