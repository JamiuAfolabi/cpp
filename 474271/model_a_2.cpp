#include <iostream>

// Forward declarations of Account classes
class SavingsAccount;
class FixedAccount;

// Visitor interface
class TaxVisitor {
public:
    virtual double visit(SavingsAccount& account) = 0;
    virtual double visit(FixedAccount& account) = 0;
};

// Base Account class
class Account {
public:
    virtual double accept(TaxVisitor& visitor) = 0; // Accept method for Visitor
    virtual double getBalance() const = 0; // Method to get balance
};

// Savings Account class
class SavingsAccount : public Account {
private:
    double balance;
public:
    SavingsAccount(double bal) : balance(bal) {}

    double accept(TaxVisitor& visitor) override {
        return visitor.visit(*this);
    }

    double getBalance() const override {
        return balance;
    }
};

// Fixed Account class
class FixedAccount : public Account {
private:
    double balance;
public:
    FixedAccount(double bal) : balance(bal) {}

    double accept(TaxVisitor& visitor) override {
        return visitor.visit(*this);
    }

    double getBalance() const override {
        return balance;
    }
};

// Concrete Visitor for Tax Calculation
class TaxCalculator : public TaxVisitor {
public:
    double visit(SavingsAccount& account) override {
        return account.getBalance() * 0.02; // 2% tax for savings accounts
    }

    double visit(FixedAccount& account) override {
        return account.getBalance() * 0.15; // 15% tax for fixed accounts
    }
};

int main() {
    // Create instances of accounts
    SavingsAccount savings(1000.0);  // Example balance for a savings account
    FixedAccount fixed(2000.0);       // Example balance for a fixed account

    // Tax calculator instance
    TaxCalculator taxCalculator;

    // Calculate taxes
    double savingsTax = savings.accept(taxCalculator);
    double fixedTax = fixed.accept(taxCalculator);

    // Output the results
    std::cout << "Savings Account Tax: $" << savingsTax << std::endl;
    std::cout << "Fixed Account Tax: $" << fixedTax << std::endl;

    return 0;
}