#include <iostream>
#include <cmath>
#include <cassert> // For assertions

using namespace std;

// Inline function to calculate compound interest
inline double calculateTotalAmount(double principal, double rate, int timesCompounded, int years) {
    return principal * pow(1 + (rate / timesCompounded), timesCompounded * years);
}

// Inline function to calculate interest
inline double calculateTotalInterest(double totalAmount, double principal) {
    return totalAmount - principal;
}

// Inline function to handle user input
inline void getUserInput(double &principal, double &annualInterestRate, int &timesCompounded, int &years) {
    cout << "Enter the principal amount: ";
    cin >> principal;

    cout << "Enter the annual interest rate (in percentage): ";
    cin >> annualInterestRate;

    cout << "Enter the number of times interest is compounded per year: ";
    cin >> timesCompounded;

    cout << "Enter the number of years: ";
    cin >> years;
}

// Inline function to output results
inline void displayResults(double totalAmount, double totalInterest) {
    cout << "Total amount after specified years: $" << totalAmount << endl;
    cout << "Total interest accumulated: $" << totalInterest << endl;
}

// Lambda function for calculating compound interest
auto calculateInterest = [](double principal, double annualInterestRate, int timesCompounded, int years) {
    double rate = annualInterestRate / 100.0;
    double totalAmount = calculateTotalAmount(principal, rate, timesCompounded, years);
    return calculateTotalInterest(totalAmount, principal);
};

void testCalculateInterest() {
    // Test cases to validate the correctness of the lambda function
    // Test 1: Basic test case
    double principal1 = 1000.0;
    double rate1 = 5.0; // 5%
    int times1 = 12; // Compounded monthly
    int years1 = 10;

    double expectedInterest1 = 628.89; // Expected interest for the given parameters
    double calculatedInterest1 = calculateInterest(principal1, rate1, times1, years1);
    assert(fabs(calculatedInterest1 - expectedInterest1) < 0.01); // Check within a small error range

    // Test 2: No interest case
    double principal2 = 1000.0;
    double rate2 = 0.0; // 0%
    int times2 = 1; // Compounded annually
    int years2 = 5;

    double expectedInterest2 = 0.0; // Expected interest for the given parameters
    double calculatedInterest2 = calculateInterest(principal2, rate2, times2, years2);
    assert(fabs(calculatedInterest2 - expectedInterest2) < 0.01); // Check within a small error range

    // Test 3: High interest case
    double principal3 = 1000.0;
    double rate3 = 10.0; // 10%
    int times3 = 1; // Compounded annually
    int years3 = 5;

    double expectedInterest3 = 610.51; // Expected interest for the given parameters
    double calculatedInterest3 = calculateInterest(principal3, rate3, times3, years3);
    assert(fabs(calculatedInterest3 - expectedInterest3) < 0.01); // Check within a small error range

    cout << "All tests passed!" << endl;
}

int main() {
    double principal, annualInterestRate;
    int timesCompounded, years;

    // Get user input
    getUserInput(principal, annualInterestRate, timesCompounded, years);

    // Calculate the total interest using the lambda function
    double totalInterest = calculateInterest(principal, annualInterestRate, timesCompounded, years);

    // Calculate the total amount separately for display
    double totalAmount = calculateTotalAmount(principal, annualInterestRate / 100.0, timesCompounded, years);

    // Display results
    displayResults(totalAmount, totalInterest);

    // Run tests
    testCalculateInterest();

    return 0;
}