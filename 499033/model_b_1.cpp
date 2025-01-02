
#include<bits/stdc++.h>
using namespace std;

double calculate_loan_fee(double loan_amount, char loan_type) {

    double fee;

    if (loan_type == 'H' || loan_type == 'h') {
        // House loan fee
        fee = loan_amount * 0.15;
    } 
    else if (loan_type == 'C' || loan_type == 'c') {
        // Car loan fee
        fee = loan_amount * 0.10;
    } 
    else {
        cout << "Invalid loan type! Please enter 'H' for House loan or 'C' for Car loan." << endl;
        return -1;
    }
    return fee;
}

int main() {
   double loan_amount;
   char loan_type;

   cout << "Enter the total loan amount: $";
   cin >> loan_amount;

   cout << "Enter the type of loan (H for House, C for Car): ";
   cin >> loan_type;

   double fee = calculate_loan_fee(loan_amount, loan_type);

   if(fee != -1)
   {
       cout << "The loan fee is: $" << fee << endl;
   }

   return 0;
}
 