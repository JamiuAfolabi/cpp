#include <iostream>
#include <iomanip>

int main() {
    double initialCapital = 1000000.0; // Initial capital in USD
    double usdToEur = 1.1586;           // USD to EUR exchange rate
    double eurToGbp = 1.4600;           // EUR to GBP exchange rate
    double usdToGbp = 1.6939;           // USD to GBP exchange rate

    // Step 1: Convert USD to EUR
    double eurAmount = initialCapital / usdToEur;

    // Step 2: Convert EUR to GBP
    double gbpAmount = eurAmount / eurToGbp;

    // Step 3: Convert GBP back to USD
    double finalUsdAmount = gbpAmount * usdToGbp;

    // Step 4: Calculate profit
    double profit = finalUsdAmount - initialCapital;

    // Output results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Initial Capital (USD): " << initialCapital << std::endl;
    std::cout << "Final Amount (USD): " << finalUsdAmount << std::endl;
    std::cout << "Profit: " << profit << std::endl;

    if (profit > 0) {
        std::cout << "Arbitrage opportunity exists!" << std::endl;
    } else {
        std::cout << "No arbitrage opportunity." << std::endl;
    }

    return 0;
}