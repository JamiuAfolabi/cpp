#include <iostream>
#include <iomanip>
#include <cmath>

// Template to calculate periodic interest rate at compile-time
template<double AnnualRate, int Frequency>
struct InterestRate {
    static constexpr double value = (AnnualRate / 100) / Frequency;
};

// Template to calculate periodic payment at compile-time
template<double LoanAmount, double AnnualRate, int Duration, int Frequency>
struct PeriodicPayment {
    static constexpr int totalPayments = Duration * Frequency;
    static constexpr double interestRate = InterestRate<AnnualRate, Frequency>::value;
    static constexpr double value = (LoanAmount * interestRate) / (1 - std::pow(1 + interestRate, -totalPayments));
};

template<double LoanAmount, double AnnualRate, int Duration, int Frequency>
class Loan {
public:
    static constexpr double periodicPayment = PeriodicPayment<LoanAmount, AnnualRate, Duration, Frequency>::value;
    
    static void generateAmortizationSchedule() {
        double remainingBalance = LoanAmount;
        double totalInterestPaid = 0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Payment\t\tPrincipal\tInterest\tBalance\n";

        for (int i = 1; i <= Duration * Frequency; ++i) {
            double interest = remainingBalance * InterestRate<AnnualRate, Frequency>::value;
            double principal = periodicPayment - interest;
            remainingBalance -= principal;
            totalInterestPaid += interest;

            std::cout << i << "\t\t"
                      << principal << "\t\t"
                      << interest << "\t\t"
                      << remainingBalance << "\n";
        }

        std::cout << "Total payments: " << Duration * Frequency << "\n";
        std::cout << "Total interest paid: $" << totalInterestPaid << "\n";
        std::cout << "Total payment: $" << (Duration * Frequency * periodicPayment) << "\n";
    }
};

int main() {
    constexpr double loanAmount = 10000; // Example loan amount
    constexpr double annualInterestRate = 5; // Example interest rate
    constexpr int loanDuration = 2; // Example loan duration in years
    constexpr int paymentFrequency = 12; // Example payment frequency (monthly)

    Loan<loanAmount, annualInterestRate, loanDuration, paymentFrequency>::generateAmortizationSchedule();

    return 0;
}