#include <iostream>
#include <string>

struct Portfolio {
    double equities;
    double bonds;
    double commodities;
    double realEstate;
    double cash;

    Portfolio(double e, double b, double c, double r, double ca)
        : equities(e), bonds(b), commodities(c), realEstate(r), cash(ca) {}

    void display() {
        std::cout << "Portfolio Allocation:\n";
        std::cout << "Equities: " << equities << "%\n";
        std::cout << "Bonds: " << bonds << "%\n";
        std::cout << "Commodities: " << commodities << "%\n";
        std::cout << "Real Estate: " << realEstate << "%\n";
        std::cout << "Cash: " << cash << "%\n";
    }
};

enum MarketCondition {
    INTEREST_RATE_CHANGE,
    INFLATION_RATE,
    VIX_INDEX,
    CURRENCY_EXCHANGE_RATE
};

__attribute__((always_inline)) 
void rebalancePortfolio(Portfolio &portfolio, MarketCondition condition) {
    switch (condition) {
        case INTEREST_RATE_CHANGE:
            // Adjust allocation between Equities and Bonds
            portfolio.equities += 5;
            portfolio.bonds -= 5;
            break;
        
        case INFLATION_RATE:
            // Alter allocation between Equities and Commodities
            portfolio.equities += 3;
            portfolio.commodities -= 3;
            break;

        case VIX_INDEX:
            // Modify Equities and Cash allocation
            portfolio.equities -= 4;
            portfolio.cash += 4;
            break;

        case CURRENCY_EXCHANGE_RATE:
            // Shift allocation between Commodities and Real Estate
            portfolio.commodities -= 2;
            portfolio.realEstate += 2;
            break;

        default:
            std::cout << "Unknown market condition." << std::endl;
            break;
    }
}

int main() {
    Portfolio myPortfolio(60.0, 20.0, 10.0, 5.0, 5.0); // Initial allocations
    myPortfolio.display();

    // Simulate market condition changes
    MarketCondition currentCondition = INTEREST_RATE_CHANGE;
    rebalancePortfolio(myPortfolio, currentCondition);
    std::cout << "\nAfter Interest Rate Change:\n";
    myPortfolio.display();

    currentCondition = INFLATION_RATE;
    rebalancePortfolio(myPortfolio, currentCondition);
    std::cout << "\nAfter Inflation Rate Change:\n";
    myPortfolio.display();

    currentCondition = VIX_INDEX;
    rebalancePortfolio(myPortfolio, currentCondition);
    std::cout << "\nAfter VIX Index Change:\n";
    myPortfolio.display();

    currentCondition = CURRENCY_EXCHANGE_RATE;
    rebalancePortfolio(myPortfolio, currentCondition);
    std::cout << "\nAfter Currency Exchange Rate Change:\n";
    myPortfolio.display();

    return 0;
}