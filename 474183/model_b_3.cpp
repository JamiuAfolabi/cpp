#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // For size_t

struct Customer {
    string *name;
    string *address;
    string *orderDetails;
};

struct MemoryAllocation {
    void* pointer;
    size_t size;
};

std::vector<MemoryAllocation> allocatedMemoryBlocks;

void inputCustomerDetails(Customer &customer) {
    customer.name = new string;
    customer.address = new string;
    customer.orderDetails = new string;

    logMemoryAllocation(customer.name, sizeof(string*));
    logMemoryAllocation(customer.address, sizeof(string*));
    logMemoryAllocation(customer.orderDetails, sizeof(string*));

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

    freeMemoryBlock(customer.name);
    freeMemoryBlock(customer.address);
    freeMemoryBlock(customer.orderDetails);
}

void logMemoryAllocation(void* pointer, size_t size) {
    MemoryAllocation allocation;
    allocation.pointer = pointer;
    allocation.size = size;
    allocatedMemoryBlocks.push_back(allocation);

    cout << "Memory allocated: " << pointer << " (" << size << " bytes)" << endl;
}

void freeMemoryBlock(void* pointer) {
    for (size_t i = 0; i < allocatedMemoryBlocks.size(); ++i) {
        if (allocatedMemoryBlocks[i].pointer == pointer) {
            allocatedMemoryBlocks.erase(allocatedMemoryBlocks.begin() + i);
            cout << "Memory deallocated: " << pointer << endl;
            return;
        }
    }

    cout << "Memory leak detected! Pointer " << pointer << " not found in allocated blocks." << endl;
}

int main() {
    int numCustomers;

    cout << "Enter number of customers: ";
    cin >> numCustomers;
    cin.ignore(); // Clear the newline character from the input buffer

    Customer *customers = new Customer[numCustomers];

    logMemoryAllocation(customers, sizeof(Customer) * numCustomers);

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

    freeMemoryBlock(customers); // Free memory for the customers array

    if (allocatedMemoryBlocks.empty()) {
        cout << "All memory allocated has been deallocated. No memory leaks.\n";
    } else {
        cout << "Memory leak detected! The following blocks were not deallocated:\n";
        for (const MemoryAllocation& block : allocatedMemoryBlocks) {
            cout << "Pointer: " << block.pointer << ", Size: " << block.size << " bytes" << endl;
        }
    }

    return 0;
}
