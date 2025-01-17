#include <iostream>
#include <vector>
#include <functional>

struct LoanDetails {
    int loanId;
    double amount;
    double interest;
    int duration; // Duration in months
    
    void display() const {
        std::cout << "Loan ID: " << loanId 
                  << ", Amount: " << amount 
                  << ", Interest: " << interest 
                  << "%, Duration: " << duration << " months" 
                  << std::endl;
    }
};

class LoanSystem {
public:
    using EventHandler = void (*)(const LoanDetails&);
    
    LoanSystem() {
        // Initialize event handlers to nullptr
        for (auto& handler : eventHandlers) {
            handler = nullptr;
        }
    }

    void registerHandler(int eventType, EventHandler handler) {
        if (eventType >= 0 && eventType < MAX_EVENTS) {
            eventHandlers[eventType] = handler;
        }
    }
    
    void triggerEvent(int eventType, const LoanDetails& loan) {
        if (eventType >= 0 && eventType < MAX_EVENTS && eventHandlers[eventType] != nullptr) {
            eventHandlers[eventType](loan);
        } else {
            std::cout << "Event not registered or does not exist." << std::endl;
        }
    }

    void addLoan(int loanId, double amount, double interest, int duration) {
        LoanDetails loan = {loanId, amount, interest, duration};
        loans.push_back(loan);
        triggerEvent(LOAN_ADDED, loan); // Triggering loan added event
    }

    void displayLoans() const {
        for (const auto& loan : loans) {
            loan.display();
        }
    }

private:
    std::vector<LoanDetails> loans;
    static const int MAX_EVENTS = 3; // For simplicity
    EventHandler eventHandlers[MAX_EVENTS];
    
    enum EventTypes {
        LOAN_ADDED = 0,
        LOAN_REMOVED,
        LOAN_UPDATED
    };
};

// Sample event handlers
void loanAddedHandler(const LoanDetails& loan) {
    std::cout << "Loan Added: ";
    loan.display();
}

void loanRemovedHandler(const LoanDetails& loan) {
    std::cout << "Loan Removed: ";
    loan.display();
}

void loanUpdatedHandler(const LoanDetails& loan) {
    std::cout << "Loan Updated: ";
    loan.display();
}

int main() {
    LoanSystem loanSystem;

    // Register event handlers
    loanSystem.registerHandler(LoanSystem::LOAN_ADDED, loanAddedHandler);
    loanSystem.registerHandler(LoanSystem::LOAN_REMOVED, loanRemovedHandler);
    loanSystem.registerHandler(LoanSystem::LOAN_UPDATED, loanUpdatedHandler);

    // Add loans
    loanSystem.addLoan(1, 5000.0, 5.0, 24);
    loanSystem.addLoan(2, 10000.0, 6.0, 36);
    
    // Display loans
    std::cout << "\nAll Loans:\n";
    loanSystem.displayLoans();

    return 0;
}