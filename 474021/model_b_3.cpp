#include <iostream>
#include <cmath>
#include <cassert>

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

// Lambda function to calculate compound interest
auto calculateCompoundInterestLambda = [](double principal, double rate, int timesCompounded, int years) {
    double totalAmount = principal * pow(1 + (rate / timesCompounded), timesCompounded * years);
    return totalAmount - principal;
};

int main() {
    double principal, annualInterestRate;
    int timesCompounded, years;

    // Get user input
    getUserInput(principal, annualInterestRate, timesCompounded, years);

    // Convert annual interest rate from percentage to decimal
    double rate = annualInterestRate / 100.0;

    // Calculate the total amount and interest using the original function
    double totalAmount = calculateTotalAmount(principal, rate, timesCompounded, years);
    double totalInterestUsingOriginal = calculateTotalInterest(totalAmount, principal);

    // Calculate interest using the lambda function
    double totalInterestUsingLambda = calculateCompoundInterestLambda(principal, rate, timesCompounded, years);

    // Test the correctness of the lambda function using assert
    assert(totalInterestUsingOriginal == totalInterestUsingLambda);

    cout << "Total amount after " << years << " years: $" << totalAmount << endl;
    cout << "Total interest accumulated: $" << totalInterestUsingLambda << endl;

    return 0;
}