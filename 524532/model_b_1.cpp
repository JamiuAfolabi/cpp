#include <iostream>
using namespace std;

struct Loan {
    double amount;
    int interestRate;
    int term; // in years
};

int main() {
    int numCustomers;
    cout << "Enter the number of customers: ";
    cin >> numCustomers;

    // Dinamically allocate memory for customer pointers
    Customer** customers = new Customer*[numCustomers];

    // Initially, all customers have no loans
    for (int i = 0; i < numCustomers; ++i) {
        customers[i] = nullptr;
    }

    // Process each customer
    for (int customerIndex = 0; customerIndex < numCustomers; ++customerIndex) {
        int numLoans;
        cout << "Enter the number of loans for customer " << (customerIndex + 1) << ": ";
        cin >> numLoans;

        // Dynamically allocate memory for the loans of the current customer
        customers[customerIndex] = new Loan[numLoans];

        // Enter loan details for the current customer
        for (int loanIndex = 0; loanIndex < numLoans; ++loanIndex) {
            cout << "Enter loan details for loan " << (loanIndex + 1) << ": ";
            cout << "  Amount: $";
            cin >> customers[customerIndex][loanIndex].amount;
            cout << "  Interest Rate: ";
            cin >> customers[customerIndex][loanIndex].interestRate;
            cout << "  Term (years): ";
            cin >> customers[customerIndex][loanIndex].term;
        }
    }

    // Display loan details for each customer
    for (int customerIndex = 0; customerIndex < numCustomers; ++customerIndex) {
        if (customers[customerIndex] != nullptr) {
            cout << "\nLoans for Customer " << (customerIndex + 1) << ": " << endl;
            int numLoans = *(numLoansPtr + customerIndex);
            for (int loanIndex = 0; loanIndex < numLoans; ++loanIndex) {
                Loan& loan = customers[customerIndex][loanIndex];
                cout << "  Loan " << (loanIndex + 1) << ": ";
                cout << "  Amount: $" << loan.amount << ", ";
                cout << "  Interest Rate: " << loan.interestRate << "%, ";
                cout << "  Term: " << loan.term << " years" << endl;
            }
        } else {
            cout << "\nCustomer " << (customerIndex + 1) << " has no loans." << endl;
        }
    }
    
    // Free the dynamically allocated memory
    for (int i = 0; i < numCustomers; ++i) {
        delete[] customers[i];
    }
    delete[] customers;

    return 0;
}