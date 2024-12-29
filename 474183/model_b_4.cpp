#include <iostream>
#include <string>
#include <memory>

struct Customer {
    std::unique_ptr<string> name;
    std::unique_ptr<string> address;
    std::unique_ptr<string> orderDetails;
};

Customer makeCustomer() {
    Customer customer;
    customer.name = std::make_unique<string>();
    customer.address = std::make_unique<string>();
    customer.orderDetails = std::make_unique<string>();

    cout << "Enter customer name: ";
    getline(cin, *customer.name);
    cout << "Enter customer address: ";
    getline(cin, *customer.address);
    cout << "Enter order details: ";
    getline(cin, *customer.orderDetails);

    return customer;
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
    cin.ignore(); 

    std::vector<Customer> customers;

    for (int i = 0; i < numCustomers; ++i) {
        cout << "\nEntering details for customer " << (i + 1) << ":" << endl;
        customers.push_back(makeCustomer());
    }

    for (int i = 0; i < numCustomers; ++i) {
        displayCustomerDetails(customers[i]);
    }

    return 0;
}