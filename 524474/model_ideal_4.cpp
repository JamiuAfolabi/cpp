#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <stack>

struct Loan {
    int loanId;
    double amount;
    double interest;
    int duration; // in months

    Loan(int id, double amt, double intRate, int dur)
        : loanId(id), amount(amt), interest(intRate), duration(dur) {}
};

class LoanSystemImpl {
public:
    bool createLoan(const Loan& loan) {
        if (loan.loanId <= 0 || loanExists(loan)) {
            return false;
        }
        loans.push_back(loan);
        return true;
    }

    bool deleteLoan(const Loan& loan) {
        auto it = std::remove_if(loans.begin(), loans.end(), [&](const Loan& l) {
            return l.loanId == loan.loanId;
        });
        if (it != loans.end()) {
            loans.erase(it, loans.end());
            return true;
        }
        return false;
    }

    bool viewLoan(const Loan& loan) {
        auto it = std::find_if(loans.begin(), loans.end(), [&](const Loan& l) {
            return l.loanId == loan.loanId;
        });
        if (it != loans.end()) {
            std::cout << "Viewing loan ID=" << it->loanId << ", Amount=" << it->amount << std::endl;
            return true;
        }
        return false;
    }

    bool loanExists(const Loan& loan) const {
        return std::any_of(loans.begin(), loans.end(), [&](const Loan& l) {
            return l.loanId == loan.loanId;
        });
    }

private:
    std::vector<Loan> loans;
};

class Command {
public:
    virtual ~Command() = default;
    
    // Virtual methods for pre/post execution hooks
    virtual bool preExecute() { return true; }   // Default behavior: no action
    virtual void postExecute() {}                 // Default behavior: no action
    
    virtual bool execute() = 0;
    virtual bool undo() = 0;
    virtual bool validate() const = 0;
    virtual std::string getClassName() const = 0;
};

class LoanSystem {
public:
    using LogEntry = std::string;

    LoanSystem(LoanSystemImpl& impl) : systemImpl(impl) {}

    void addCommand(std::shared_ptr<Command> command) {
        if (command->validate()) {
            commandHistory.push(command);
            if (command->preExecute()) {
                command->execute();
                command->postExecute();
                log("Executed command: " + command->getClassName());
            } else {
                log("Pre-execution failed for command: " + command->getClassName());
            }
            // Clear redo stack after a new command is added
            redoStack = std::stack<std::shared_ptr<Command>>(); // Clear redo stack
        } else {
            log("Validation failed for command: " + command->getClassName());
        }
    }

    void undo() {
        if (!commandHistory.empty()) {
            auto command = commandHistory.top();
            commandHistory.pop();
            command->undo();
            redoStack.push(command);
            log("Undone command: " + command->getClassName());
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            auto command = redoStack.top();
            redoStack.pop();
            if (command->preExecute()) {
                command->execute();
                command->postExecute();
                commandHistory.push(command);
                log("Redone command: " + command->getClassName());
            }
        }
    }

    void printLogs() const {
        std::cout << "Logs:\n";
        for (const auto& entry : logs) {
            std::cout << entry << std::endl;
        }
    }

private:
    LoanSystemImpl& systemImpl;
    std::vector<LogEntry> logs;
    std::stack<std::shared_ptr<Command>> commandHistory;
    std::stack<std::shared_ptr<Command>> redoStack;

    void log(const LogEntry& message) {
        logs.push_back(message);
    }
};

class CreateLoanCommand : public Command {
public:
    CreateLoanCommand(LoanSystem& system, LoanSystemImpl& systemImpl, const Loan& loan)
        : system(system), systemImpl(systemImpl), loan(loan) {}

    bool preExecute() override {
        std::cout << "Preparing to create loan ID=" << loan.loanId << std::endl;
        return loan.loanId > 0; // Ensure valid loan ID
    }

    bool execute() override {
        if (!systemImpl.createLoan(loan)) {
            return false;
        }
        return true;
    }

    void postExecute() override {
        std::cout << "Loan ID=" << loan.loanId << " created successfully." << std::endl;
    }

    bool undo() override {
        return systemImpl.deleteLoan(loan);
    }

    bool validate() const override {
        return loan.loanId > 0;
    }

    std::string getClassName() const override {
        return "CreateLoanCommand";
    }

private:
    LoanSystem& system;
    LoanSystemImpl& systemImpl;
    Loan loan;
};

class DeleteLoanCommand : public Command {
public:
    DeleteLoanCommand(LoanSystem& system, LoanSystemImpl& systemImpl, const Loan& loan)
        : system(system), systemImpl(systemImpl), loan(loan) {}

    bool preExecute() override {
        std::cout << "Preparing to delete loan ID=" << loan.loanId << std::endl;
        return systemImpl.loanExists(loan);
    }

    bool execute() override {
        return systemImpl.deleteLoan(loan);
    }

    void postExecute() override {
        std::cout << "Loan ID=" << loan.loanId << " deleted successfully." << std::endl;
    }

    bool undo() override {
        return systemImpl.createLoan(loan);
    }

    bool validate() const override {
        return systemImpl.loanExists(loan);
    }

    std::string getClassName() const override {
        return "DeleteLoanCommand";
    }

private:
    LoanSystem& system;
    LoanSystemImpl& systemImpl;
    Loan loan;
};

class ViewLoanCommand : public Command {
public:
    ViewLoanCommand(LoanSystem& system, LoanSystemImpl& systemImpl, const Loan& loan)
        : system(system), systemImpl(systemImpl), loan(loan) {}

    bool preExecute() override {
        std::cout << "Preparing to view loan ID=" << loan.loanId << std::endl;
        return systemImpl.loanExists(loan);
    }

    bool execute() override {
        return systemImpl.viewLoan(loan);
    }

    void postExecute() override {
        std::cout << "Completed viewing loan ID=" << loan.loanId << std::endl;
    }

    bool undo() override {
        return true; // No undo needed for viewing
    }

    bool validate() const override {
        return systemImpl.loanExists(loan);
    }

    std::string getClassName() const override {
        return "ViewLoanCommand";
    }

private:
    LoanSystem& system;
    LoanSystemImpl& systemImpl;
    Loan loan;
};

int main() {
    LoanSystemImpl loanSystemImpl;
    LoanSystem system(loanSystemImpl);

    Loan loan1(1, 5000.0, 5.0, 12);
    Loan loan2(2, 10000.0, 4.0, 24);
    
    auto createLoanCmd1 = std::make_shared<CreateLoanCommand>(system, loanSystemImpl, loan1);
    auto viewLoanCmd1 = std::make_shared<ViewLoanCommand>(system, loanSystemImpl, loan1);
    auto deleteLoanCmd1 = std::make_shared<DeleteLoanCommand>(system, loanSystemImpl, loan1);
    
    system.addCommand(createLoanCmd1);
    system.addCommand(viewLoanCmd1);
    system.addCommand(deleteLoanCmd1);
    
    system.undo();  // Undo delete
    system.redo();  // Redo delete
    
    system.printLogs();
}
