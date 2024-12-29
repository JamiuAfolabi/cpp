#include <iostream>
#include <cmath>

using namespace std;

// float sufficient precision
// Optionally, double if needed
using Currency = float;

inline Currency principal(Currency inputs) {
    cout << "Enter the principal amount: ";
    cin >> inputs;
    return inputs;
}

inline Currency annualInterestRate(Currency inputs) {
    cout << "Enter the annual interest rate (in percentage): ";
    cin >> inputs;
    return inputs;
}

inline int timesCompounded(int inputs) {
    cout << "Enter the number of times interest is compounded per year: ";
    cin >> inputs;
    return inputs;
}

inline int years(int inputs) {
    cout << "Enter the number of years: ";
    cin >> inputs;
    return inputs;
}
inline Currency convertPercentageToDecimal(Currency rate) {
    return rate / 100.0;
}

inline Currency calculateCompoundInterest(
        Currency principal, Currency rate, int n, int years) {

    return principal * pow(1 + (rate / n), n * years);
}


inline Currency calculateTotalInterest(Currency totalAmount, Currency principal) {
    return totalAmount - principal;
}

int main() {
    Currency principalAmount = principal(0);
    Currency annualInterest = annualInterestRate(0);
    int compoundTimes = timesCompounded(0);
    int numYears = years(0);

    Currency rate = convertPercentageToDecimal(annualInterest);

    // Calculate the total amount using the compound interest formula
    Currency totalAmount = calculateCompoundInterest(principalAmount, rate, compoundTimes, numYears);

    // Calculate the total interest accumulated
    Currency totalInterest = calculateTotalInterest(totalAmount, principalAmount);
    // Output the results
    cout << "Total amount after " << numYears << " years: $" << totalAmount << endl;
    cout << "Total interest accumulated: $" << totalInterest << endl;
    return 0;
}