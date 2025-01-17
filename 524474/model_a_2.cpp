#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>

struct Loan {
    int loanId;
    double amount;
    double interest;
    int duration; // in months

    Loan(int id, double amt, double intRate, int dur)
        : loanId(id), amount(amt), interest(intRate), duration(dur) {}
};

class LoanSystem {
public:
    using LoanEventHandler = std::function<void(const Loan&)>;
    
    // Map of event names to their respective handlers
    std::unordered_map<std::string, std::vector<LoanEventHandler>> eventHandlers;

    // Method to register an event with a handler
    void registerLoanEvent(const std::string& eventName, LoanEventHandler handler) {
        eventHandlers[eventName].push_back(handler);
    }

    // Method to trigger an event by name
    void triggerEvent(const std::string& eventName, const Loan& loan) {
        auto it = eventHandlers.find(eventName);
        if (it != eventHandlers.end()) {
            for (const auto& handler : it->second) {
                handler(loan);
            }
        } else {
            std::cout << "Event '" << eventName << "' not found!" << std::endl;
        }
    }

    // Method to remove a handler for a specific event
    void removeLoanEvent(const std::string& eventName, LoanEventHandler handler) {
        auto it = eventHandlers.find(eventName);
        if (it != eventHandlers.end()) {
            auto& handlers = it->second;
            handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                [&handler](const LoanEventHandler& h) { return h.target<void(const Loan&)>() == handler.target<void(const Loan&)>(); }),
                handlers.end());
        } else {
            std::cout << "Event '" << eventName << "' not found!" << std::endl;
        }
    }

    // Loan operations
    void createLoan(const Loan& loan) {
        loans.push_back(loan);
        std::cout << "Loan created: ID=" << loan.loanId << ", Amount=" << loan.amount 
                  << ", Interest=" << loan.interest << ", Duration=" << loan.duration << " months." << std::endl;
    }

    void viewLoan(const Loan& loan) {
        auto it = std::find_if(loans.begin(), loans.end(), [&](const Loan& l) { return l.loanId == loan.loanId; });
        if (it != loans.end()) {
            std::cout << "Loan details: ID=" << it->loanId << ", Amount=" << it->amount 
                      << ", Interest=" << it->interest << ", Duration=" << it->duration << " months." << std::endl;
        } else {
            std::cout << "Loan not found." << std::endl;
        }
    }

    void deleteLoan(const Loan& loan) {
        auto it = std::remove_if(loans.begin(), loans.end(), [&](const Loan& l) { return l.loanId == loan.loanId; });
        if (it != loans.end()) {
            loans.erase(it, loans.end());
            std::cout << "Loan deleted: ID=" << loan.loanId << std::endl;
        } else {
            std::cout << "Loan not found." << std::endl;
        }
    }

private:
    std::vector<Loan> loans; // Vector to store loans
};

int main() {
    LoanSystem loanSystem;

    // Register event handlers
    loanSystem.registerLoanEvent("create", [&](const Loan& loan) { loanSystem.createLoan(loan); });
    loanSystem.registerLoanEvent("view", [&](const Loan& loan) { loanSystem.viewLoan(loan); });
    loanSystem.registerLoanEvent("delete", [&](const Loan& loan) { loanSystem.deleteLoan(loan); });

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);

    // Trigger events to create loans
    loanSystem.triggerEvent("create", loan1);
    loanSystem.triggerEvent("create", loan2);

    // View loan details
    loanSystem.triggerEvent("view", loan1);

    // Delete a loan
    loanSystem.triggerEvent("delete", loan1);

    // Try to view the deleted loan
    loanSystem.triggerEvent("view", loan1);

    return 0;
}