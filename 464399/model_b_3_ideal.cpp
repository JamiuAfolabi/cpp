#include <iostream>
#include <iomanip>
#include <cmath>

// Template class for Loan
template<typename LoanType>
class Loan {
public:
    static constexpr int paymentFrequency = LoanType::paymentFrequency;
    static constexpr int loanTermYears = LoanType::loanTermYears;

    Loan(double principal, double annualRate) : principal(principal), annualRate(annualRate) {}

    double calculateMonthlyPayment() {
        double monthlyRate = annualRate / 100.0 / paymentFrequency;
        int numPayments = loanTermYears * paymentFrequency;
        return (principal * monthlyRate) / (1 - pow(1 + monthlyRate, -numPayments));
    }

    void generatePaymentSchedule() {
        double monthlyPayment = calculateMonthlyPayment();
        double monthlyRate = annualRate / 100.0 / paymentFrequency;
        int numPayments = loanTermYears * paymentFrequency;

        double balance = principal;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Payment Schedule:\n";
        std::cout << "---------------------------------------\n";
        std::cout << "Payment # | Payment | Interest | Principal | Balance\n";
        std::cout << "---------------------------------------\n";

        for (int i = 1; i <= numPayments; ++i) {
            double interest = balance * monthlyRate;
            double principalPayment = monthlyPayment - interest;
            balance -= principalPayment;

            std::cout << std::setw(9) << i
                      << " | " << std::setw(7) << monthlyPayment
                      << " | " << std::setw(8) << interest
                      << " | " << std::setw(9) << principalPayment
                      << " | " << std::setw(7) << (balance < 0 ? 0 : balance)
                      << std::endl;
        }

        std::cout << "---------------------------------------\n";
    }

private:
    double principal;
    double annualRate;
};

// Custom Loan Types using Structs and Type Aliases (for better readability)
struct FixedRateLoanMonthlyType {
    static constexpr int paymentFrequency = 12;   // Monthly payments
    static constexpr int loanTermYears = 30;      // Default loan term (can be changed)
};

struct FixedRateLoanQuarterlyType {
    static constexpr int paymentFrequency = 4;    // Quarterly payments
    static constexpr int loanTermYears = 25;      // Default loan term (can be changed)
};

// Create type aliases for loan types
using FixedRateLoanMonthly = Loan<FixedRateLoanMonthlyType>;
using FixedRateLoanQuarterly = Loan<FixedRateLoanQuarterlyType>;


int main() {
    double principal;
    double annualRate;

    std::cout << "Enter loan principal amount: ";
    std::cin >> principal;

    std::cout << "Enter annual interest rate (as a percentage): ";
    std::cin >> annualRate;
    
    // Selected loan type: FixedRateLoanMonthly
    FixedRateLoanMonthly loan(principal, annualRate);

    // Generate payment schedule
    loan.generatePaymentSchedule();

    return 0;
}
