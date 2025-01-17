#include <iostream>
#include <iomanip>
#include <cmath>
#include <type_traits>

template<typename T>
constexpr T power(T base, int exponent) {
    T result = 1;

    // Handle negative exponents
    if (exponent < 0) {
        base = 1 / base;
        exponent = -exponent;
    }

    // Iterative loop to multiply the base by itself exponent times
    for (; exponent > 0; --exponent) {
        result *= base;
    }

    return result;
}
// Compile-time loan calculator
template<
    unsigned long long LoanAmountCents,                // Loan amount in cents
    unsigned long long InterestBps,                    // Interest rate in basis points (1/10000th)
    unsigned int Years,                                 // Loan duration in years
    unsigned int PaymentsPerYear,                       // Payments per year
    typename = std::enable_if_t<(Years >= 1 && Years <= 40)> // Compile-time check for Years
>
class LoanCalculator {
    // Convert from internal fixed-point representation to double
    static constexpr double to_dollars(unsigned long long cents) {
        return static_cast<double>(cents) / 100.0;
    }
    
    static constexpr double to_decimal_rate(unsigned long long bps) {
        return static_cast<double>(bps) / 10000.0;
    }
    
    static constexpr double loan_amount = to_dollars(LoanAmountCents);
    static constexpr double annual_rate = to_decimal_rate(InterestBps);
    static constexpr double periodic_rate = annual_rate / PaymentsPerYear;
    static constexpr int total_payments = Years * PaymentsPerYear;
    
public:
    // Calculate periodic payment at compile time
    static constexpr double periodic_payment() {
        double rate = periodic_rate;
        double amount = loan_amount;
        return (amount * rate) / (1 - power(1 + rate, -total_payments));
    }
    
    // Generate amortization schedule
    static void generate_schedule() {
        constexpr double payment = periodic_payment();
        double balance = loan_amount;
        double total_interest = 0;
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Loan Amount: $" << loan_amount << "\n";
        std::cout << "Annual Rate: " << (annual_rate * 100) << "%\n";
        std::cout << "Term: " << Years << " years\n";
        std::cout << "\nPayment\t\tPrincipal\tInterest\tBalance\n";
        
        for (int i = 1; i <= total_payments; ++i) {
            double interest = balance * periodic_rate;
            double principal = payment - interest;
            balance -= principal;
            total_interest += interest;
            
            std::cout << i << "\t\t" 
                      << principal << "\t\t" 
                      << interest << "\t\t" 
                      << balance << "\n";
        }
        
        std::cout << "\nLoan Summary (computed at compile-time):\n";
        std::cout << "Monthly Payment: $" << payment << "\n";
        std::cout << "Total Payments: " << total_payments << "\n";
        std::cout << "Total Interest: $" << total_interest << "\n";
        std::cout << "Total Amount: $" << (payment * total_payments) << "\n";
    }
    
    // Getters for compile-time constants
    static constexpr double get_periodic_rate() { return periodic_rate; }
    static constexpr int get_total_payments() { return total_payments; }
    static constexpr double get_loan_amount() { return loan_amount; }
};

int main() {
    // Example: $100,000 loan at 5.5% for 30 years with monthly payments
    // 10000000 = $100,000.00 (in cents)
    // 550 = 5.50% (in basis points)
    using MyLoan = LoanCalculator<10000000, 550, 30, 12>; // Valid: 30 years
    // using MyLoan = LoanCalculator<10000000, 550, 50, 12>; // Invalid: Uncommenting this will cause a compile-time error.

    // All these values are computed at compile-time
    constexpr double payment = MyLoan::periodic_payment();
    constexpr double rate = MyLoan::get_periodic_rate();
    constexpr int total_payments = MyLoan::get_total_payments();
    
    std::cout << "Compile-time calculated monthly payment: $" << payment << "\n\n";
    
    // Generate the full schedule
    MyLoan::generate_schedule();
    
    return 0;
}