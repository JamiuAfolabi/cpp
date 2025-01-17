#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>

struct Loan {
    int loanId;
    double amount;
    double interest;
    int duration;

    Loan(int id, double amt, double intRate, int dur)
        : loanId(id), amount(amt), interest(intRate), duration(dur) {}
};

class LoanSystem {
public:
    // Typedef for function pointer type
    using LoanEventHandler = std::function<void(const Loan&)>;

    LoanSystem() {}

    // Register a new event handler
    void registerLoanEvent(const std::string& eventName, LoanEventHandler handler) {
        eventHandlers[eventName].push_back(handler);
    }

    // Remove an event handler
    void removeLoanEvent(const std::string& eventName, LoanEventHandler handler) {
        eventHandlers[eventName].erase(std::remove(eventHandlers[eventName].begin(), eventHandlers[eventName].end(), handler), eventHandlers[eventName].end());
    }

    // Trigger an event by name
    void triggerEvent(const std::string& eventName, const Loan& loan) {
        auto it = eventHandlers.find(eventName);
        if (it != eventHandlers.end()) {
            for (const auto& handler : it->second) {
                handler(loan);
            }
        } else {
            std::cout << "Event not found!" << std::endl;
        }
    }

    // Loan operations (these are now private methods since they are event handlers)
    private:
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
    std::map<std::string, std::vector<LoanEventHandler>> eventHandlers;
    std::vector<Loan> loans;
};

int main() {
    LoanSystem loanSystem;

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);

    // Register event handlers
    loanSystem.registerLoanEvent("create", std::bind(&LoanSystem::createLoan, &loanSystem, std::placeholders::_1));
    loanSystem.registerLoanEvent("view", std::bind(&LoanSystem::viewLoan, &loanSystem, std::placeholders::_1));
    loanSystem.registerLoanEvent("delete", std::bind(&LoanSystem::deleteLoan, &loanSystem, std::placeholders::_1));