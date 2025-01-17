#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand and srand

#define NUM_EVENTS 3  // Available loan events

// Structure to store loan details
struct Loan {
    int loanId;
    double amount;
    double interestRate;
    int duration;
};

// Enum for different loan events
enum LoanEvent {
    LOAN_APPLIED,
    LOAN_SANCTIONED,
    LOAN_REPAID,
};

// Type definition for function pointers for different loan operations
typedef void (*LoanEventHandler)(const Loan&);

class LoanSystem {
private:
    std::vector<Loan> loans;
    LoanEventHandler handlers[NUM_EVENTS];

public:
    LoanSystem() {
        for (int i = 0; i < NUM_EVENTS; i++) {
            handlers[i] = nullptr;
        }
    }

    // Function to register a handler for a specific event
    void registerHandler(LoanEvent event, LoanEventHandler handler) {
        switch (event) {
            case LOAN_APPLIED:
                handlers[0] = handler;
                break;
            case LOAN_SANCTIONED:
                handlers[1] = handler;
                break;
            case LOAN_REPAID:
                handlers[2] = handler;
                break;
            default:
                std::cerr << "Invalid loan event.\n";
        }
    }

    // Function to trigger a loan event with the specified details
    void triggerEvent(LoanEvent event, const Loan& loan) {
        if (handlers[event] != nullptr) {
            handlers[event](loan);
        } else {
            std::cout << "No handler registered for the event.\n";
        }
    }

    // Sample utility function to generate random details for a loan
    Loan generateRandomLoan() {
        Loan loan;
        loan.loanId = loans.size() + 1;
        loan.amount = 100000 + (rand() % 400001);
        loan.interestRate = 5 + (rand() % 6);
        loan.duration = 1 + (rand() % 11);
        return loan;
    }

    // Function to apply a new loan (this will trigger LOAN_APPLIED event)
    void applyLoan() {
        Loan newLoan = generateRandomLoan();
        loans.push_back(newLoan);
        std::cout << "Loan applied with ID: " << newLoan.loanId << "\n";
        triggerEvent(LOAN_APPLIED, newLoan);
    }

    // Sample print event handler function
    void printEventHandler(const Loan& loan) const {
        std::cout << "**Event Handler:**\n";
        switch (loan.interestRate) {
            case LOAN_APPLIED:
                std::cout << "Loan Applied:\n";
                break;
            case LOAN_SANCTIONED: 
                std::cout << "Loan Sanctioned:\n";
                break;
            case LOAN_REPAID:
                std::cout << "Loan Repaid:\n";
                break;
        }
        std::cout << "Loan ID: " << loan.loanId << ", Amount: " << loan.amount
                  << ", Interest Rate: " << loan.interestRate << "%, Duration: "
                  << loan.duration << " years\n";
    }

    
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Seeding the random number generator
    LoanSystem loanSystem;

    // Register event handlers
    loanSystem.registerHandler(LOAN_APPLIED, &LoanSystem::printEventHandler);
    loanSystem.registerHandler(LOAN_SANCTIONED, &LoanSystem::printEventHandler);  
    loanSystem.registerHandler(LOAN_REPAID, &LoanSystem::printEventHandler);

    loanSystem.applyLoan(); // Trigger LOAN_APPLIED event
    loanSystem.applyLoan(); // Trigger LOAN_APPLIED event


    //  (Additional code to simulate loan sanction and repayment
    //ing can be placed here to trigger other events)

    return 0;
}
