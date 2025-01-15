
#include <iostream>
using namespace std;

enum MarketCondition {
    INTEREST_RATE_CHANGE,
    INFLATION_RATE,
    VIX_INDEX,
    USD_JPY_EXCHANGE_RATE
};

struct Portfolio {
    double equities;
    double bonds;
    double commodities;
    double cash;
    double realEstate;

    void rebalance(MarketCondition condition, double change) {
        switch (condition) {
            case INTEREST_RATE_CHANGE:
                rebalanceInterestRate(change);
                break;
            case INFLATION_RATE:
                rebalanceInflationRate(change);
                break;
            case VIX_INDEX:
                rebalanceVIXIndex(change);
                break;
            case USD_JPY_EXCHANGE_RATE:
                rebalanceUSDJPYExchangeRate(change);
                break;
        }
    }

private:
    // Rebalancing actions for each market condition
    void rebalanceInterestRate(double change) {
        // Adjust allocation between Equities and Bonds
        if (change > 0) {  // Increasing interest rates
            equities -= change;
            bonds += change;
        } else {  // Decreasing interest rates
            equities += change;
            bonds -= change;
        }
    }

    void rebalanceInflationRate(double change) {
        // Alter allocation between Equities and Commodities
        if (change > 0) {  // Increasing inflation
            equities -= change;
            commodities += change;
        } else {  // Decreasing inflation
            equities += change;
            commodities -= change;
        }
    }

    void rebalanceVIXIndex(double change) {
        // Modify Equities and Cash allocation
        if (change > 0) {  // Increasing market volatility
            equities -= change;
            cash += change;
        } else {  // Decreasing market volatility
            equities += change;
            cash -= change;
        }
    }

    void rebalanceUSDJPYExchangeRate(double change) {
        // Shift allocation between Commodities and Real Estate
        if (change > 0) {  // Appreciating USD against JPY
            commodities -= change;
            realEstate += change;
        } else {  // Depreciating USD against JPY
            commodities += change;
            realEstate -= change;
        }
    }
};

int main() {
    Portfolio portfolio;
    portfolio.equities = 0.3;
    portfolio.bonds = 0.4;
    portfolio.commodities = 0.2;
    portfolio.cash = 0.05;
    portfolio.realEstate = 0.05;

    portfolio.rebalance(MarketCondition::INFLATION_RATE, 0.05);  // Example usage

    // Display the updated portfolio allocation
    cout << "Updated Portfolio Allocation:" << endl;
    cout << "Equities: " << portfolio.equities << endl;
    cout << "Bonds: " << portfolio.bonds << endl;
    cout << "Commodities: " << portfolio.commodities << endl;
    cout << "Cash: " << portfolio.cash << endl;
    cout << "Real Estate: " << portfolio.realEstate << endl;

    return 0;
}
  