#include <iostream>
#include <limits>
#include <cstdio>
using namespace std;

#define INVALID_INPUT_ERROR   "Invalid input. Please enter a numeric value.\n"
#define NON_POSITIVE_ERROR    "Input must be greater than zero.\n"

double getValidInput(const char* prompt) {
    double input;
    while (true) {
        printf("%s", prompt);
        cin >> input;
        if (cin.fail()) {
            printf(INVALID_INPUT_ERROR);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (input <= 0) {
            printf(NON_POSITIVE_ERROR);
        } else {
            return input;
        }
    }
}

int main() {
    double totalExpectedPayments = getValidInput("Enter the total expected payments: ");
    double totalPaymentsMade = getValidInput("Enter the total payments made: ");
    string status = determineIFRS9Status(totalExpectedPayments, totalPaymentsMade);
    printf("The IFRS 9 status is: %s\n", status.c_str());

    return 0;
}

string determineIFRS9Status(double totalExpectedPayments, double totalPaymentsMade) {
    if (totalPaymentsMade < 0) {
        return "Invalid payment amount";
    }
    
    if (totalPaymentsMade == 0) {
        return "Default";
    }

    double paymentRatio = totalPaymentsMade / totalExpectedPayments;
    if (paymentRatio < 0.5) {
        return "High Risk / Default";
    } else if (paymentRatio >= 0.5 && paymentRatio < 1.0) {
        return "Performing";
    } else {
        return "Performing / Fully Paid";
    }
}