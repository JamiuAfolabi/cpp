#include <iostream>
#include <vector>
#include <functional>

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
    // Typedef for function pointer type
    typedef void (LoanSystem::*LoanEventHandler)(const Loan&);

    // Array of function pointers for loan operations
    LoanEventHandler loanEventHandlers[3];

    LoanSystem() {
        // Register event handlers
        loanEventHandlers[0] = &LoanSystem::createLoan;
        loanEventHandlers[1] = &LoanSystem::viewLoan;
        loanEventHandlers[2] = &LoanSystem::deleteLoan;
    }

    void registerLoanEvent(int eventId, LoanEventHandler handler) {
        loanEventHandlers[eventId] = handler;
    }

    void triggerEvent(int eventId, const Loan& loan) {
        if (eventId >= 0 && eventId < 3) {
            (this->*loanEventHandlers[eventId])(loan);
        } else {
            std::cout << "Invalid event ID!" << std::endl;
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

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);

    // Create loans
    loanSystem.triggerEvent(0, loan1);
    loanSystem.triggerEvent(0, loan2);

    // View loan details
    loanSystem.triggerEvent(1, loan1);

    // Delete a loan
    loanSystem.triggerEvent(2, loan1);

    // Try to view the deleted loan
    loanSystem.triggerEvent(1, loan1);

    return 0;
}