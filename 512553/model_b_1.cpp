
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    // Initial capital in USD
    double initial_capital = 1000000.00;

    // Exchange rates
    double usd_eur = 1.1586;
    double eur_gbp = 1.4600;
    double usd_gbp = 1.6939;

   //Financial conversion for optimising potential profit
    double usd_from_eur = 1.0 / usd_eur;
    double gbp_from_eur = 1.0 / eur_gbp;
    double eur_from_gbp = 1.0 / usd_gbp;


    double profit_from_xyz = pow(usr_from_eur * eur_from_gbp * gbp_from_usd, 10) - 1;

    if(profit_from_xyz > 0.0)
    {
       cout << "Triangle arbitrage opportunity detected!" << endl;
       cout << "Maximum profit percentage: " << profit_from_xyz << "%" << endl;
    
       double amount_usd = initial_capital;

       // Convert USD to EUR
       double amount_eur = amount_usd * usd_eur;
 
       // Convert EUR to GBP
       double amount_gbp = amount_eur * eur_gbp;
       double total = amount_gbp * usd_gbp;

       double profit = total - initial_capital;
       cout << "Final profit after execution: " << profit << " USD" <<endl;

    }
    else
    {
       cout << "Triangle arbitrage opportunity not present." << endl;
    }
    
    return 0;
} 
