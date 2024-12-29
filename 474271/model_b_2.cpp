#include <iostream>
using namespace std;

class Account {
public:
    virtual ~Account() = default;
    virtual double getBalance() = 0;
    virtual void accept(class TaxCalculator*) = 0;
};

class SavingsAccount : public Account {
public:
    SavingsAccount(double balance) : balance{balance} { }
    double getBalance() override { return balance; }
    void accept(TaxCalculator* calculator) override {
        calculator->visit(*this);
    }
private:
    double balance;
};

class FixedAccount : public Account {
public:
    FixedAccount(double balance) : balance{balance} { }
    double getBalance() override { return balance; }
    void accept(TaxCalculator* calculator) override {
        calculator->visit(*this);
    }
private:
    double balance;
};

class TaxCalculator {
public:
    virtual double calculateTax(const Account&) = 0;
    void visit(SavingsAccount& a) {
        tax = calculateTax(a);
    }
    void visit(FixedAccount& a) {
        tax = calculateTax(a);
    }
    double getTax() const { return tax; }

protected:
    double tax = 0.0;
};

class SimpleTaxCalculator : public TaxCalculator {
public:
    double calculateTax(const Account& account) override {
        if (const SavingsAccount& savings = dynamic_cast<const SavingsAccount&>(account)) {
            return savings.getBalance() * 0.02;
        } 
        else if (const FixedAccount& fixed = dynamic_cast<const FixedAccount&>(account)) {
            return fixed.getBalance() * 0.15;
        }
        return 0.0; // Return zero for other types (assertion would be better)
    }
};

int main() {
    double savingsBalance = 1000.0;
    double fixedBalance = 2000.0;

    Account* accounts[] = {
        new SavingsAccount(savingsBalance),
        new FixedAccount(fixedBalance)
    };

    for (auto* account : accounts) {
        SimpleTaxCalculator calculator;
        account->accept(&calculator);
        cout << "Tax: $" << calculator.getTax() << endl;
        delete account; // Don't forget to delete dynamically allocated objects
    }

    return 0;
}