#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>

enum class LoanOperationResult {
    SUCCESS,
    ERROR
};

struct LoanOperation {
    LoanOperationResult operator()(const Loan& loan) const {
        return SUCCESS;
    }
};

class LoanSystem {
public:
    using LoanEventHandler = std::function<LoanOperationResult(const Loan&)>;
    using LoanOperationChain = std::vector<LoanEventHandler>;

    // Map of event names to their respective operation chains
    std::unordered_map<std::string, LoanOperationChain> eventHandlers;

    // Method to register an event with a handler
    void registerLoanEvent(const std::string& eventName, LoanEventHandler handler) {
        eventHandlers[eventName].push_back(handler);
    }

    // Method to trigger an event by name
    LoanOperationResult triggerEvent(const std::string& eventName, const Loan& loan) {
        auto it = eventHandlers.find(eventName);
        if (it != eventHandlers.end()) {
            LoanOperationChain& handlers = it->second;
            for (const auto& handler : handlers) {
                LoanOperationResult result = handler(loan);
                if (result == LoanOperationResult::ERROR) {
                    return result; // Break the chain on error
                }
            }
            return LoanOperationResult::SUCCESS;
        } else {
            std::cout << "Event '" << eventName << "' not found!" << std::endl;
            return LoanOperationResult::ERROR;
        }
    }

    // Loan operations

    // Simple logging operation
    class LogLoanOperation : public LoanOperation {
    public:
        LoanOperationResult operator()(const Loan& loan) const override {
            std::cout << "Log: Loan details: ID=" << loan.loanId << ", Amount=" << loan.amount 
                      << ", Interest=" << loan.interest << ", Duration=" << loan.duration << " months." << std::endl;
            return LoanOperationResult::SUCCESS;
        }
    };

    // Create loan operation
    class CreateLoanOperation : public LoanOperation {
    private:
        std::vector<Loan>& loans;

    public:
        CreateLoanOperation(std::vector<Loan>& loans) : loans(loans) {}

        LoanOperationResult operator()(const Loan& loan) const override {
            loans.push_back(loan);
            std::cout << "Loan created: ID=" << loan.loanId << ", Amount=" << loan.amount << std::endl;
            return LoanOperationResult::SUCCESS;
        }
    };

    // Check loan balance operation (returns error for negative balance)
    class CheckLoanBalanceOperation : public LoanOperation {
    public:
        LoanOperationResult operator()(const Loan& loan) const override {
            if (loan.amount < 0) {
                std::cout << "Error: Loan amount cannot be negative." << std::endl;
                return LoanOperationResult::ERROR;
            }
            return LoanOperationResult::SUCCESS;
        }
    };

private:
    std::vector<Loan> loans; // Vector to store loans
};

int main() {
    LoanSystem loanSystem;

    // Register event handlers for loan creation
    loanSystem.registerLoanEvent("create", [&](const Loan& loan) {
        LoanSystem::CheckLoanBalanceOperation checkBalance;
        if (checkBalance(loan) == LoanOperationResult::ERROR) {
            return LoanOperationResult::ERROR;
        }

        LoanSystem::CreateLoanOperation createLoan(loanSystem.loans);
        return createLoan(loan);
    });

    loanSystem.registerLoanEvent("create", LoanSystem::LogLoanOperation{});

    // Register event handlers for loan viewing
    loanSystem.registerLoanEvent("view", LoanSystem::LogLoanOperation{});

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, -10000.0, 4.0, 24); // Negative amount to trigger error

    // Trigger events to create loans
    loanSystem.triggerEvent("create", loan1);
    loanSystem.triggerEvent("create", loan2);

    // View loan details
    loanSystem.triggerEvent("view", loan1);

    return 0;
}
