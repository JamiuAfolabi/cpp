
#include<bits/stdc++.h>
using namespace std;

//Function prototypes
double calculateLongTermLoanFee(double principal, double interestRate);
double calculateShortTermLoanFee(double principal, double interestRate);

//Main function
int main()
{
    int loanType;
    double principal, interestRate, totalFee;

    //Function pointer array
    typedef double (*feeCalculator)(double, double);
    feeCalculator feeCalcFuncs[] = {calculateLongTermLoanFee, calculateShortTermLoanFee};

    cout << "Enter loan type (1 for long-term, 2 for short-term): ";
    cin >> loanType;

    if (loanType < 1 || loanType > 2)
    {
        cout << "Invalid loan type. Please enter 1 or 2." << endl;
        return 1;
    }

    cout << "Enter the loan principal: ";
    cin >> principal;

    cout << "Enter the interest rate: ";
    cin >> interestRate;

    //Select the appropriate function pointer from the array
    feeCalculator calcFee = feeCalcFuncs[loanType - 1];

    totalFee = calcFee(principal, interestRate);

    cout << "Loan fee: $" << totalFee << endl;

    return 0;
}

// Functions to calculate fees for long-term and short-term loans
double calculateLongTermLoanFee(double principal, double interestRate)
{
    return principal * (interestRate / 100);
}

double calculateShortTermLoanFee(double principal, double interestRate)
{
    return principal * (interestRate / 50);
}
