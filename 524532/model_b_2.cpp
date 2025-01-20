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
            // Allocate array of pointers to Loan arrays
            customers = new Loan*[numCustomers];
            // Allocate array to track number of loans per customer
            loansPerCustomer = new int[numCustomers]();  // Initialize to 0
            
            // Initialize all customer pointers to nullptr
            for(int i = 0; i < numCustomers; i++) {
                customers[i] = nullptr;
            }
        } catch (const std::bad_alloc& e) {
            // Clean up any allocated memory before rethrowing
            delete[] customers;
            delete[] loansPerCustomer;
            throw std::runtime_error("Memory allocation failed during initialization");
        }
    }
    
    // Add loans for a specific customer
    void addLoansForCustomer(int customerId, Loan* loans, int numLoans) {
        if (customerId < 0 || customerId >= numCustomers) {
            throw std::out_of_range("Invalid customer ID");
        }
        
        try {
            // Allocate new array for customer's loans
            Loan* customerLoans = new Loan[numLoans];
            
            // Copy loans using pointer arithmetic
            Loan* dest = customerLoans;
            Loan* src = loans;
            for(int i = 0; i < numLoans; i++) {
                *dest++ = *src++;
            }
            
            // Delete existing loans if any
            if (customers[customerId] != nullptr) {
                delete[] customers[customerId];
            }
            
            // Update customer's loans
            customers[customerId] = customerLoans;
            loansPerCustomer[customerId] = numLoans;
            
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Memory allocation failed while adding loans");
        }
    }
    
    // Get pointer to loans for a specific customer
    Loan* getCustomerLoans(int customerId, int& numLoans) {
        if (customerId < 0 || customerId >= numCustomers) {
            throw std::out_of_range("Invalid customer ID");
        }
        
        numLoans = loansPerCustomer[customerId];
        return customers[customerId];
    }
    
    // Print all loans for debugging
    void printAllLoans() {
        for(int i = 0; i < numCustomers; i++) {
            std::cout << "Customer " << i << " loans:\n";
            if (customers[i] == nullptr) {
                std::cout << "  No loans\n";
                continue;
            }
            
            Loan* customerLoans = customers[i];
            for(int j = 0; j < loansPerCustomer[i]; j++) {
                std::cout << "  Loan " << j << ": Amount=" << customerLoans[j].amount
                         << ", Rate=" << customerLoans[j].interestRate
                         << ", Term=" << customerLoans[j].termMonths << " months\n";
            }
        }
    }

    void processMonthlyPayments() {
        for(int i = 0; i < numCustomers; i++) {
            if (customers[i] != nullptr) {
                Loan* currentLoan = customers[i];
                int numLoansForCustomer = loansPerCustomer[i];
                for (int loanIndex = 0; loanIndex < numLoansForCustomer; loanIndex++, currentLoan++) {
                    // Calculate monthly payment here
                    // For demonstration purposes, just printing the loan details
                    std::cout << "Processing loan for Customer " << i << ", Loan " << loanIndex << ":\n";
                    std::cout << "  Amount: " << currentLoan->amount << "\n";
                    std::cout << "  Interest Rate: " << currentLoan->interestRate << "\n";
                    std::cout << "  Term Months: " << currentLoan->termMonths << "\n";
                    // Add actual payment calculation logic here
                }
            }
        }
    }
    
    ~CustomerLoans() {
        // Clean up all allocated memory
        for(int i = 0; i < numCustomers; i++) {
            delete[] customers[i];
        }
        delete[] customers;
        delete[] loansPerCustomer;
    }

};

// Example usage
int main() {
    try {
        CustomerLoans loanSystem(3);  // System with 3 customers
        
        // Add loans for customer 0
        Loan customer0Loans[] = {
            {10000, 5.5, 36},
            {5000, 4.5, 24}
        };
        loanSystem.addLoansForCustomer(0, customer0Loans, 2);
        
        // Add loan for customer 1
        Loan customer1Loans[] = {
            {15000, 6.0, 48}
        };
        loanSystem.addLoansForCustomer(1, customer1Loans, 1);
        
        // Print all loans
        loanSystem.printAllLoans();
        
        // Access loans for a specific customer
        int numLoans;
        Loan* customer0LoanPtr = loanSystem.getCustomerLoans(0, numLoans);
        std::cout << "\nFirst loan amount for customer 0: " << customer0LoanPtr->amount << std::endl;
        
        // Process monthly payments for all customers
        loanSystem.processMonthlyPayments();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
