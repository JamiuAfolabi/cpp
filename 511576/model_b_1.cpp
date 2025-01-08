
#include <iostream>
#include <string>
using namespace std;

class Loan {
public:
    enum class LoanType { CAR, HOUSE };
    Loan(LoanType type, double amount) : type(type), amount(amount) {}

    virtual double calculateFee() const {
        switch (type) {
            case LoanType::CAR:
                return calculateCarLoanFee();
            case LoanType::HOUSE:
                return calculateHouseLoanFee();
        }
    }

protected:
    LoanType type;
    double amount;

    double calculateCarLoanFee() const {
        return amount * 0.015;
    }

    double calculateHouseLoanFee() const {
        return amount * 0.03;
    }
};

class CarLoan : public Loan {
public:
    CarLoan(double amount) : Loan(LoanType::CAR, amount) {}
};

class HouseLoan : public Loan {
public:
    HouseLoan(double amount) : Loan(LoanType::HOUSE, amount) {}
};

int main() {
    double carLoanAmount = 20000;
    double houseLoanAmount = 500000;

    CarLoan carLoan(carLoanAmount);
    HouseLoan houseLoan(houseLoanAmount);

    double carLoanFee = carLoan.calculateFee();
    double houseLoanFee = houseLoan.calculateFee();

    cout << "Loan Fee for Car Loan of $" << carLoanAmount << ": $" << carLoanFee << endl;
    cout << "Loan Fee for House Loan of $" << houseLoanAmount << ": $" << houseLoanFee << endl;

    return 0;
}

