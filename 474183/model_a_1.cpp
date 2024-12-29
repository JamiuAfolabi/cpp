#include <iostream>
#include <string>
using namespace std;

struct Customer {
    string *name;
    string *address;
    string *orderDetails;
};

void inputCustomerDetails(Customer &customer) {
    customer.name = new string;
    customer.address = new string;
    customer.orderDetails = new string;

    cout << "Enter customer name: ";
    getline(cin, *customer.name);
    cout << "Enter customer address: ";
    getline(cin, *customer.address);
    cout << "Enter order details: ";
    getline(cin, *customer.orderDetails);
}

void displayCustomerDetails(const Customer &customer) {
    cout << "\nCustomer Details:" << endl;
    cout << "Name: " << *customer.name << endl;
    cout << "Address: " << *customer.address << endl;
    cout << "Order Details: " << *customer.orderDetails << endl;
}

void freeMemory(Customer &customer) {
    delete customer.name;
    delete customer.address;
    delete customer.orderDetails;
}

int main() {
    int numCustomers;

    cout << "Enter number of customers: ";
    cin >> numCustomers;
    cin.ignore(); // Clear the newline character from the input buffer

    Customer *customers = new Customer[numCustomers];

    // Input details for each customer
    for (int i = 0; i < numCustomers; ++i) {
        cout << "\nEntering details for customer " << (i + 1) << ":" << endl;
        inputCustomerDetails(customers[i]);
    }

    // Display details for each customer
    for (int i = 0; i < numCustomers; ++i) {
        displayCustomerDetails(customers[i]);
        freeMemory(customers[i]); // Free memory for each customer's details
    }

    delete[] customers; // Free memory for the customers array

    return 0;
}