#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <stack>
#include <algorithm>

// Loan structure
struct Loan {
    int loanId;
    double amount;
    double interest;
    int duration; // in months

    Loan(int id, double amt, double intRate, int dur)
        : loanId(id), amount(amt), interest(intRate), duration(dur) {}
};

// Command interface
class Command {
public:
    virtual ~Command() {}
    virtual bool execute() = 0;
    virtual void undo() = 0;
    virtual std::string getName() const = 0;
};

// LoanSystem class
class LoanSystem {
public:
    void createLoan(const Loan& loan);
    void viewLoan(const Loan& loan);
    void deleteLoan(const Loan& loan);
    
    void log(const std::string& message);
    void printLogs() const;

    // Command history
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();

private:
    std::vector<Loan> loans; // Vector to store loans
    std::vector<std::string> logs; // Log messages
    std::stack<std::unique_ptr<Command>> commandHistory; // Undo stack
    std::stack<std::unique_ptr<Command>> redoStack; // Redo stack
};

// Concrete command classes
class CreateLoanCommand : public Command {
public:
    CreateLoanCommand(LoanSystem& system, const Loan& loan)
        : m_system(system), m_loan(loan) {}

    bool execute() override {
        m_system.createLoan(m_loan);
        return true;
    }

    void undo() override {
        m_system.deleteLoan(m_loan);
    }

    std::string getName() const override {
        return "CreateLoanCommand";
    }

private:
    LoanSystem& m_system;
    Loan m_loan;
};

class ViewLoanCommand : public Command {
public:
    ViewLoanCommand(LoanSystem& system, const Loan& loan)
        : m_system(system), m_loan(loan) {}

    bool execute() override {
        m_system.viewLoan(m_loan);
        return true;
    }

    void undo() override {
        // No operation to undo viewing a loan
    }

    std::string getName() const override {
        return "ViewLoanCommand";
    }

private:
    LoanSystem& m_system;
    Loan m_loan;
};

class DeleteLoanCommand : public Command {
public:
    DeleteLoanCommand(LoanSystem& system, const Loan& loan)
        : m_system(system), m_loan(loan) {}

    bool execute() override {
        m_system.deleteLoan(m_loan);
        return true;
    }

    void undo() override {
        m_system.createLoan(m_loan);
    }

    std::string getName() const override {
        return "DeleteLoanCommand";
    }

private:
    LoanSystem& m_system;
    Loan m_loan;
};

// Composite command for transactions
class CompositeCommand : public Command {
public:
    CompositeCommand(const std::string& name) : commandName(name) {}

    void addCommand(std::unique_ptr<Command> command) {
        commands.push_back(std::move(command));
    }

    bool execute() override {
        for (const auto& command : commands) {
            if (!command->execute()) {
                undo(); // undo if one fails
                return false;
            }
        }
        return true;
    }

    void undo() override {
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            (*it)->undo();
        }
    }

    std::string getName() const override {
        return commandName;
    }

private:
    std::vector<std::unique_ptr<Command>> commands;
    std::string commandName;
};

// Implementation of LoanSystem's methods
void LoanSystem::createLoan(const Loan& loan) {
    loans.push_back(loan);
    log("Loan created: ID=" + std::to_string(loan.loanId) + 
        ", Amount=" + std::to_string(loan.amount));
}

void LoanSystem::viewLoan(const Loan& loan) {
    auto it = std::find_if(loans.begin(), loans.end(), [&](const Loan& l) {
        return l.loanId == loan.loanId;
    });
    if (it != loans.end()) {
        log("Viewing loan ID=" + std::to_string(it->loanId) + 
            ", Amount=" + std::to_string(it->amount));
    } else {
        log("Failed to view loan: Loan ID=" + std::to_string(loan.loanId) + " not found.");
    }
}

void LoanSystem::deleteLoan(const Loan& loan) {
    auto it = std::remove_if(loans.begin(), loans.end(), [&](const Loan& l) {
        return l.loanId == loan.loanId;
    });
    if (it != loans.end()) {
        loans.erase(it, loans.end());
        log("Loan deleted: ID=" + std::to_string(loan.loanId));
    } else {
        log("Failed to delete loan: Loan ID=" + std::to_string(loan.loanId) + " not found.");
    }
}

void LoanSystem::log(const std::string& message) {
    logs.push_back(message);
}

void LoanSystem::printLogs() const {
    std::cout << "Logs:\n";
    for (const auto& entry : logs) {
        std::cout << entry << std::endl;
    }
}

void LoanSystem::executeCommand(std::unique_ptr<Command> command) {
    if (command->execute()) {
        commandHistory.push(std::move(command));
        // Clear the redo stack after a new command
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
}

void LoanSystem::undo() {
    if (!commandHistory.empty()) {
        auto command = std::move(commandHistory.top());
        command->undo();
        redoStack.push(std::move(commandHistory.top()));
        commandHistory.pop();
    }
}

void LoanSystem::redo() {
    if (!redoStack.empty()) {
        auto command = std::move(redoStack.top());
        command->execute();
        commandHistory.push(std::move(redoStack.top()));
        redoStack.pop();
    }
}

int main() {
    LoanSystem loanSystem;

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);

    // Create commands for operations
    loanSystem.executeCommand(std::make_unique<CreateLoanCommand>(loanSystem, loan1));
    loanSystem.executeCommand(std::make_unique<ViewLoanCommand>(loanSystem, loan1));
    loanSystem.executeCommand(std::make_unique<CreateLoanCommand>(loanSystem, loan2));
    loanSystem.executeCommand(std::make_unique<ViewLoanCommand>(loanSystem, loan2));

    // Undo last command
    loanSystem.undo();

    // Redo last undone command
    loanSystem.redo();

    // Composite command example
    CompositeCommand transaction("Create Loans Transaction");
    transaction.addCommand(std::make_unique<CreateLoanCommand>(loanSystem, Loan(3, 8000.0, 5.5, 12)));
    transaction.addCommand(std::make_unique<CreateLoanCommand>(loanSystem, Loan(4, 9000.0, 6.0, 15)));
    loanSystem.executeCommand(std::make_unique<CompositeCommand>(transaction));

    // Print logs
    loanSystem.printLogs();

    return 0;
}