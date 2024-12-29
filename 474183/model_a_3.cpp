#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

struct MemoryBlock {
    void* address;
    size_t size; // Size of the allocated memory block
};

struct Customer {
    string *name;
    string *address;
    string *orderDetails;
};

vector<MemoryBlock> memoryLog; // Vector to log all memory blocks

void logMemoryAllocation(void* address, size_t size) {
    memoryLog.push_back({address, size});
    printf("Allocated %zu bytes at address %p\n", size, address);
}

void logMemoryDeallocation(void* address) {
    for (auto it = memoryLog.begin(); it != memoryLog.end(); ++it) {
        if (it->address == address) {
            printf("Deallocated %zu bytes at address %p\n", it->size, it->address);
            memoryLog.erase(it);
            return;
        }
    }
    printf("Warning: Attempted to deallocate memory at %p not found in log\n", address);
}

void inputCustomerDetails(Customer &customer) {
    customer.name = new string;
    customer.address = new string;
    customer.orderDetails = new string;

    logMemoryAllocation(static_cast<void*>(customer.name), sizeof(string));
    logMemoryAllocation(static_cast<void*>(customer.address), sizeof(string));
    logMemoryAllocation(static_cast<void*>(customer.orderDetails), sizeof(string));

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

    logMemoryDeallocation(customer.name);
    logMemoryDeallocation(customer.address);
    logMemoryDeallocation(customer.orderDetails);
}

int main() {
    int numCustomers;

    cout << "Enter number of customers: ";
    cin >> numCustomers;
    cin.ignore(); // Clear the newline character from the input buffer

    Customer *customers = new Customer[numCustomers];
    logMemoryAllocation(static_cast<void*>(customers), sizeof(Customer) * numCustomers);

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
    logMemoryDeallocation(customers); // Log memory deallocation of customers array

    if (memoryLog.empty()) {
        printf("All memory allocated has been deallocated. No memory leaks.\n");
    } else {
        printf("Memory leak detected! Remaining allocations: %lu\n", memoryLog.size());
    }

    return 0;
}