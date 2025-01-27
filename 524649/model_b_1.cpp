#include<iostream>

using namespace std;

int main()
{
    double balance = 1000.00; // Initial account balance
    double withdrawal;

    cout << "\nWelcome to the ATM!\n";

    do
    {
        cout << "Enter your withdrawal amount: ";
        cin >> withdrawal;

        // Error handling for invalid input (non-numeric value)
        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid amount.\n";
            continue;
        }

        // Check for insufficient funds
        if (withdrawal <= 0)
        {
            cout << "Withdrawal amount must be positive.\n";
        }
        else if (withdrawal > balance)
        {
            cout << "Insufficient funds. Transaction declined.\n";
        }
        else
        {
            // Process withdrawal
            balance -= withdrawal;
            cout << "\nWithdrawal Successful!" << endl;
            cout << "-------------------------" << endl;
            printf("Account Balance: %.2lf\n", balance);
            printf("Withdrawal Amount: %.2lf\n", withdrawal);
            printf("Final Balance: %.2lf\n", balance);
            cout << "-------------------------\n";
        }

    } while (true); // Continue looping for more transactions

    cout << "\nThank you for using the ATM. Have a great day!\n";
    return 0;
}