#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

struct Customer {
    unique_ptr<string> name;
    unique_ptr<string> address;
    unique_ptr<string> orderDetails;
};

void inputCustomerDetails(Customer &customer) {
    customer.name = make_unique<string>();
    customer.address = make_unique<string>();
    customer.orderDetails = make_unique<string>();

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

int main() {
    int numCustomers;
    cout << "Enter number of customers: ";
    cin >> numCustomers;
    cin.ignore(); // Clear the newline character from the input buffer

    vector<Customer> customers(numCustomers); // Using vector to manage customers

    // Input details for each customer
    for (int i = 0; i < numCustomers; ++i) {
        cout << "\nEntering details for customer " << (i + 1) << ":" << endl;
        inputCustomerDetails(customers[i]);
    }

    // Display details for each customer
    for (const auto& customer : customers) {
        displayCustomerDetails(customer);
    }

    return 0; // All memory will be released automatically
}