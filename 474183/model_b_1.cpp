#include <iostream>
#include <string>
using namespace std;

// Customer structure
struct Customer {
    string name;
    string address;
    string orderDetails;
};

int main() {
    int numCustomers;
    cout << "Enter the number of customers: ";
    cin >> numCustomers;

    // Check if input is valid
    if (numCustomers <= 0) {
        cout << "Invalid number of customers. Please enter a positive integer." << endl;
        return 1;
    }

    // Dynamically allocate memory for an array of Customer pointers
    Customer** customers = new Customer*[numCustomers];

    for (int i = 0; i < numCustomers; i++) {
        customers[i] = new Customer; // Allocate memory for each customer struct

        cout << "Customer " << i + 1 << ":" << endl;
        cout << "Name: ";
        getline(cin, customers[i]->name);
        cout << "Address: ";
        getline(cin, customers[i]->address);
        cout << "Order Details: ";
        getline(cin, customers[i]->orderDetails);
    }

    // Display customer details
    cout << "\nCustomer Details:" << endl;
    displayCustomers(customers, numCustomers);

    // Free the dynamically allocated memory
    for (int i = 0; i < numCustomers; i++) {
        delete customers[i];
    }
    delete[] customers;

    cout << "Memory freed. Program exiting..." << endl;
    return 0;
}

void displayCustomers(Customer** customers, int numCustomers) {
    for (int i = 0; i < numCustomers; i++) {
        cout << "Customer " << i + 1 << ":" << endl;
        cout << "Name: " << customers[i]->name << endl;
        cout << "Address: " << customers[i]->address << endl;
        cout << "Order Details: " << customers[i]->orderDetails << endl;
        cout << "--------------------" << endl;
    }
}