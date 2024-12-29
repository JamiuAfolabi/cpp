
#include<bits/stdc++.h>
using namespace std;

int main()
{
    float totalAmount, paidAmount, currentAmount;
    char status;

    // User input for the total amount
    cout << "Enter the total amount of the loan or asset: ";
    cin >> totalAmount;

    // User input for the total amount paid so far
    cout << "Enter the total amount paid so far: ";
    cin >> paidAmount;

    if(paidAmount > totalAmount)
    {
        cout << "\nPlease enter a valid paid amount less than or equal to the total amount.\n";
        return 0;
    }

    currentAmount = totalAmount - paidAmount;

    cout << "\nCurrent Amount Due: " << fixed << setprecision(2) << currentAmount << endl;

    if(currentAmount <= 0)
    {
        cout << "\nThe asset is performing as expected. No default under IFRS 9.\n";
        status = 'P'; // Performing
    }
    else
    {
        cout << "\nThe loan or asset is in default under IFRS 9.\n";
        status = 'D'; // Default
    }

    // Display the IFRS 9 status
    cout << "\nIFRS 9 Status: " << status << endl;

    return 0;
}
