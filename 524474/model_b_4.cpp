#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

// Command base class
class LoanCommand {
public:
    using LoanId = int;
    virtual ~LoanCommand() = default;

    virtual void execute(LoanSystem& loanSystem) = 0;
    virtual void undo(LoanSystem& loanSystem) = 0;

    virtual bool validate(const LoanSystem& loanSystem) const = 0;
};

// Create Loan Command
class CreateLoanCommand : public LoanCommand {
public:
    CreateLoanCommand(LoanId loanId, double amount, double interest, int duration)
        : loanId(loanId), amount(amount), interest(interest), duration(duration) {}

    void execute(LoanSystem& loanSystem) override {
        if (validate(loanSystem)) {
            loanSystem.createLoan(loanId, amount, interest, duration);
        }
    }

    void undo(LoanSystem& loanSystem) override {
        loanSystem.deleteLoan(loanId);
    }

    bool validate(const LoanSystem& loanSystem) const override {
        return loanId > 0 && !loanSystem.findLoan(loanId);
    }

private:
    LoanId loanId;
    double amount;
    double interest;
    int duration;
};

// View Loan Command
class ViewLoanCommand : public LoanCommand {
public:
    ViewLoanCommand(LoanId loanId) : loanId(loanId) {}

    void execute(LoanSystem& loanSystem) override {
        if (validate(loanSystem)) {
            loanSystem.viewLoan(loanId);
        }
    }

    void undo(LoanSystem& loanSystem) override {}

    bool validate(const LoanSystem& loanSystem) const override {
        return loanSystem.findLoan(loanId);
    }

private:
    LoanId loanId;
};

// Delete Loan Command
class DeleteLoanCommand : public LoanCommand {
public:
    DeleteLoanCommand(LoanId loanId) : loanId(loanId) {}

    void execute(LoanSystem& loanSystem) override {
        if (validate(loanSystem)) {
            loanSystem.deleteLoan(loanId);
        }
    }

    void undo(LoanSystem& loanSystem) override {
        loanSystem.createLoan(loanId, loanSystem.findLoanDetails(loanId).amount, loanSystem.findLoanDetails(loanId).interest, loanSystem.findLoanDetails(loanId).duration);
    }

    bool validate(const LoanSystem& loanSystem) const override {
        return loanSystem.findLoan(loanId);
    }

private:
    LoanId loanId;
};

// Composite Command for multiple actions
class CompositeCommand : public LoanCommand {
public:
    void execute(LoanSystem& loanSystem) override {
        for (const auto& cmd : commands) {
            cmd->execute(loanSystem);
        }
    }

    void undo(LoanSystem& loanSystem) override {
        for (const auto& cmd : commands) {
            cmd->undo(loanSystem);
        }
    }

    bool validate(const LoanSystem& loanSystem) const override {
        for (const auto& cmd : commands) {
            if (!cmd->validate(loanSystem)) {
                return false;
            }
        }
        return true;
    }

    void addCommand(std::shared_ptr<LoanCommand> command) {
        commands.push_back(command);
    }

private:
    std::vector<std::shared_ptr<LoanCommand>> commands;
};

class LoanSystem {
public:
    using LoanId = LoanCommand::LoanId;
    using CommandPtr = std::shared_ptr<LoanCommand>;

    // Command execution
    void executeCommand(CommandPtr command) {
        if (command->validate(*this)) {
            command->execute(*this);
            history.push_back(command);
        } else {
            std::cout << "Command validation failed." << std::endl;
        }
    }