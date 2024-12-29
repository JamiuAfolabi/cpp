#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// Template for calculating interest
template <typename InterestPolicy>
class Loan {
public:
    Loan(double principal, double annualRate, int years)
        : principal_(principal), annualRate_(annualRate), years_(years) {}

    inline double calculateMonthlyPayment() const {
        double monthlyRate = annualRate_ / 100.0 / 12.0; // Convert annual rate to monthly
        int numPayments = years_ * 12; // Total number of payments
        return (principal_ * monthlyRate) / (1 - pow(1 + monthlyRate, -numPayments));
    }

    inline void generatePaymentSchedule() const {
        double monthlyPayment = calculateMonthlyPayment();
        double monthlyRate = annualRate_ / 100.0 / 12.0;
        int numPayments = years_;

        double balance = principal_;
        cout << fixed << setprecision(2);
        cout << "Payment Schedule:\n";
        cout << "---------------------------------------\n";
        cout << "Payment # | Payment | Interest | Principal | Balance\n";
        cout << "---------------------------------------\n";

        for (int i = 1; i <= numPayments; ++i) {
            double interest = balance * monthlyRate;
            double principalPayment = monthlyPayment - interest;
            balance -= principalPayment;

            cout << setw(9) << i 
                 << " | " << setw(7) << monthlyPayment 
                 << " | " << setw(8) << interest 
                 << " | " << setw(9) << principalPayment 
                 << " | " << setw(7) << (balance < 0 ? 0 : balance) 
                 << endl;
        }

        cout << "---------------------------------------\n";
    }

private:
    double principal_;
    double annualRate_;
    int years_;
};

// Client code
int main() {
    double principal;
    double annualRate;
    int years;

    cout << "Enter loan principal amount: ";
    cin >> principal;

    cout << "Enter annual interest rate (as a percentage): ";
    cin >> annualRate;

    cout << "Enter loan term in years: ";
    cin >> years;

    Loan<double> loan(principal, annualRate, years);
    loan.generatePaymentSchedule();

    return 0;
}