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

    do {
        printf("Enter the total expected payments: ");
        scanf("%lf", &totalExpectedPayments);

        if (totalExpectedPayments <= 0) {
            printf("Total expected payments must be greater than zero. Please try again.\n");
        }
    } while (totalExpectedPayments <= 0);

    do {
        printf("Enter the total payments made: ");
        scanf("%lf", &totalPaymentsMade);

        if (totalPaymentsMade < 0) {
            printf("Total payments made cannot be negative. Please try again.\n");
        }
    } while (totalPaymentsMade < 0);

    // Determine and print the IFRS 9 status
    string status = determineIFRS9Status(totalExpectedPayments, totalPaymentsMade);
    printf("The IFRS 9 status is: %s\n", status.c_str());

    return 0;
}