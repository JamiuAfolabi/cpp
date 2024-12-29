
#include<bits/stdc++.h>
using namespace std;

 double compoundInterest(double principal, double annual_rate, double compound_times, int years)
{
    double rate = annual_rate / compound_times;
    double interest;
    double amount = principal;

    for (int i = 0; i < years; i++)
    {
        interest = amount * pow(1 + rate, compound_times) - amount;
        amount = amount + interest;
    }
 
    return amount - principal;
}

int main()
{
    double principal, annual_rate, compound_times, interest;

    cout << "Enter the principal amount: ";
    cin >> principal;

    cout << "Enter the annual interest rate (in %): ";
    cin >> annual_rate;
    annual_rate /= 100;

    cout << "Enter the number of times interest is compounded per year: ";
    cin >> compound_times;

    cout << "Enter the number of years: ";
    int years;
    cin >> years;

    interest = compoundInterest(principal, annual_rate, compound_times, years);

    cout << "Total Interest Accumulated: $" << interest << endl;
    return 0;
}
