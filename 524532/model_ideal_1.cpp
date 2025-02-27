#include <iostream>
#include <stdexcept>

struct Loan {
    double amount;
    double interestRate;
    int termMonths;
};

class CustomerLoans {
private:
    Loan** customers;       // Pointer to array of pointers to Loan arrays
    int* loansPerCustomer;  // Array to store number of loans for each customer
    int numCustomers;       // Total number of customers

public:
    CustomerLoans(int initialCustomers) : numCustomers(initialCustomers) {
        try {
            customers = new Loan*[numCustomers];
            loansPerCustomer = new int[numCustomers];
            
            // Initialize using pointer arithmetic
            Loan** custPtr = customers;
            int* loanCountPtr = loansPerCustomer;
            for(int i = 0; i < numCustomers; i++) {
                *custPtr++ = nullptr;
                *loanCountPtr++ = 0;
            }
        } catch (const std::bad_alloc& e) {
            delete[] customers;
            delete[] loansPerCustomer;
            throw std::runtime_error("Memory allocation failed during initialization");
        }
    }
    
    void addLoansForCustomer(int customerId, Loan* loans, int numLoans) {
        if (customerId < 0 || customerId >= numCustomers) {
            throw std::out_of_range("Invalid customer ID");
        }
        
        try {
            Loan* customerLoans = new Loan[numLoans];
            
            // Copy loans using pure pointer arithmetic
            Loan* dest = customerLoans;
            Loan* src = loans;
            Loan* const end = src + numLoans;
            
            while(src < end) {
                *dest++ = *src++;
            }
            
            // Free existing loans if any using pointer arithmetic
            Loan** customerPtr = customers + customerId;
            if (*customerPtr != nullptr) {
                delete[] *customerPtr;
            }
            
            *customerPtr = customerLoans;
            *(loansPerCustomer + customerId) = numLoans;
            
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Memory allocation failed while adding loans");
        }
    }
    
    Loan* getCustomerLoans(int customerId, int& numLoans) {
        if (customerId < 0 || customerId >= numCustomers) {
            throw std::out_of_range("Invalid customer ID");
        }
        
        numLoans = *(loansPerCustomer + customerId);
        return *(customers + customerId);
    }
    
    void printAllLoans() {
        Loan** custPtr = customers;
        int* loanCountPtr = loansPerCustomer;
        
        for(int i = 0; i < numCustomers; i++, custPtr++, loanCountPtr++) {
            std::cout << "Customer " << i << " loans:\n";
            if (*custPtr == nullptr) {
                std::cout << "  No loans\n";
                continue;
            }
            
            // Use pointer arithmetic to iterate through loans
            Loan* loanPtr = *custPtr;
            Loan* const endPtr = loanPtr + *loanCountPtr;
            
            int loanIndex = 0;
            while(loanPtr < endPtr) {
                std::cout << "  Loan " << loanIndex++ << ": "
                         << "Amount=" << loanPtr->amount
                         << ", Rate=" << loanPtr->interestRate
                         << ", Term=" << loanPtr->termMonths << " months\n";
                loanPtr++;
            }
        }
    }
    
    ~CustomerLoans() {
        Loan** custPtr = customers;
        for(int i = 0; i < numCustomers; i++) {
            delete[] *custPtr++;
        }
        delete[] customers;
        delete[] loansPerCustomer;
    }
};

int main() {
    try {
        CustomerLoans loanSystem(3);  // System with 3 customers
        
        // Add loans for customer 0
        Loan initialLoans[] = {
            {10000, 5.5, 36},
            {5000, 4.5, 24}
        };
        loanSystem.addLoansForCustomer(0, initialLoans, 2);
        
        // Add loan for customer 1
        Loan customer1Loans[] = {
            {15000, 6.0, 48}
        };
        loanSystem.addLoansForCustomer(1, customer1Loans, 1);
        
        loanSystem.printAllLoans();
        
        // Access loans using pointer arithmetic
        int numLoans;
        Loan* retrievedLoans = loanSystem.getCustomerLoans(0, numLoans);
        Loan* firstLoan = retrievedLoans;
        std::cout << "\nFirst loan amount for customer 0: " << firstLoan->amount << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}