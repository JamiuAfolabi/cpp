#include <iostream>
#include <string>
#include <map>
#include <cstdlib>

using namespace std;

// Global map to track memory allocations
map<void*, size_t> memoryTracker;

// Overloaded new operator for tracking memory allocations
void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        memoryTracker[ptr] = size;  // Track the allocation
        printf("Allocated %zu bytes at %p\n", size, ptr);
    }
    return ptr;
}

// Overloaded delete operator for tracking memory deallocations
void operator delete(void* ptr) noexcept {
    if (ptr) {
        printf("Deallocated %zu bytes from %p\n", memoryTracker[ptr], ptr);
        memoryTracker.erase(ptr);  // Remove from the tracker
        free(ptr);
    }
}

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

    // Output remaining allocations if any
    if (!memoryTracker.empty()) {
        printf("Memory leaks detected:\n");
        for (const auto& entry : memoryTracker) {
            printf("Leaked %zu bytes from %p\n", entry.second, entry.first);
        }
    } else {
        printf("No memory leaks detected.\n");
    }

    return 0;
}