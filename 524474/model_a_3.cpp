#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

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
    using LoanEventHandler = std::function<bool(const Loan&)>; // Returns true to continue the chain
    
    struct LogEntry {
        std::string message;
        // You can add timestamp or other info later if needed
    };

    // Map of loan operations
    std::vector<LoanEventHandler> operationChain;
    std::vector<LogEntry> logs;

    void addToChain(LoanEventHandler handler) {
        operationChain.push_back(handler);
    }

    // Triggers the loan operations in sequence
    void triggerChain(const Loan& loan) {
        for (const auto& handler : operationChain) {
            if (!handler(loan)) {
                log("Operation failed, stopping chain.");
                break; // If handler returns false, stop processing.
            }
        }
    }

    // Loan operations with logging and error handling
    bool createLoan(const Loan& loan) {
        // Simulate error handling (for instance, loanId must be unique)
        if (loan.loanId <= 0) {
            log("Failed to create loan: Invalid loan ID.");
            return false; // stop the chain
        }

        loans.push_back(loan);
        log("Loan created: ID=" + std::to_string(loan.loanId) + 
            ", Amount=" + std::to_string(loan.amount));
        return true; // continue the chain
    }

    bool viewLoan(const Loan& loan) {
        auto it = std::find_if(loans.begin(), loans.end(), [&](const Loan& l) {
            return l.loanId == loan.loanId;
        });
        if (it != loans.end()) {
            log("Viewing loan ID=" + std::to_string(it->loanId) + 
                ", Amount=" + std::to_string(it->amount));
            return true; // continue the chain
        } else {
            log("Failed to view loan: Loan ID=" + std::to_string(loan.loanId) + " not found.");
            return false; // stop the chain
        }
    }

    bool deleteLoan(const Loan& loan) {
        auto it = std::remove_if(loans.begin(), loans.end(), [&](const Loan& l) {
            return l.loanId == loan.loanId;
        });
        if (it != loans.end()) {
            loans.erase(it, loans.end());
            log("Loan deleted: ID=" + std::to_string(loan.loanId));
            return true; // continue the chain
        } else {
            log("Failed to delete loan: Loan ID=" + std::to_string(loan.loanId) + " not found.");
            return false; // stop the chain
        }
    }

    void printLogs() const {
        std::cout << "Logs:\n";
        for (const auto& entry : logs) {
            std::cout << entry.message << std::endl;
        }
    }

private:
    std::vector<Loan> loans; // Vector to store loans

    // Log function
    void log(const std::string& message) {
        logs.push_back({message});
    }
};

int main() {
    LoanSystem loanSystem;

    // Add operations to the chain
    loanSystem.addToChain([&](const Loan& loan) { return loanSystem.createLoan(loan); });
    loanSystem.addToChain([&](const Loan& loan) { return loanSystem.viewLoan(loan); });
    loanSystem.addToChain([&](const Loan& loan) { return loanSystem.deleteLoan(loan); });

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);
    Loan invalidLoan(-1, 3000.0, 6.0, 12); // For testing error handling

    // Trigger event chain for valid loan
    loanSystem.triggerChain(loan1);
    loanSystem.triggerChain(loan2);

    // Trigger event chain for an invalid loan
    loanSystem.triggerChain(invalidLoan);

    // View logs
    loanSystem.printLogs();

    // Try to view a deleted loan
    loanSystem.triggerChain(loan1);

    return 0;
}