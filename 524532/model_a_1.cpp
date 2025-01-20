#include <iostream>

struct Loan {
    double amount;
    double interestRate;
    int term; // in months
};

struct Customer {
    Loan* loans; // Pointer to an array of loans
    int numLoans; // Number of loans for this customer
};

int main() {
    // Example: Create 3 customers
    const int numberOfCustomers = 3;
    Customer* customers = new Customer[numberOfCustomers];

    // Example loan distribution
    customers[0].numLoans = 2; // Customer 0 has 2 loans
    customers[1].numLoans = 3; // Customer 1 has 3 loans
    customers[2].numLoans = 1; // Customer 2 has 1 loan

    // Allocate memory for the loans using pointer arithmetic
    for (int i = 0; i < numberOfCustomers; ++i) {
        customers[i].loans = new Loan[customers[i].numLoans];
    }

    // Example data input for loans
    customers[0].loans[0] = {10000, 5.5, 60}; // Loan 1 for Customer 0
    customers[0].loans[1] = {5000, 4.5, 36};  // Loan 2 for Customer 0

    customers[1].loans[0] = {20000, 4.0, 48}; // Loan 1 for Customer 1
    customers[1].loans[1] = {15000, 6.0, 60}; // Loan 2 for Customer 1
    customers[1].loans[2] = {7000, 3.5, 24};  // Loan 3 for Customer 1

    customers[2].loans[0] = {30000, 5.0, 72}; // Loan 1 for Customer 2

    // Output the loan details
    for (int i = 0; i < numberOfCustomers; ++i) {
        std::cout << "Customer " << i+1 << " ha %s " << customers[i].numLoans << " loans:\n";
        for (int j = 0; j < customers[i].numLoans; ++j) {
            Loan loan = customers[i].loans[j];
            std::cout << "  Loan " << j+1 << ": Amount = " << loan.amount
                      << ", Interest Rate = " << loan.interestRate
                      << ", Term = " << loan.term << " months\n";
        }
    }

    // Cleanup: Free the allocated memory
    for (int i = 0; i < numberOfCustomers; ++i) {
        delete[] customers[i].loans; // Free the loans for each customer
    }
    delete[] customers; // Free the customers array

    return 0;
}