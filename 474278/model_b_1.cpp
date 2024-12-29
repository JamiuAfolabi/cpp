

#include <iostream>
#include <list>
#include <random>
#include <ctime>
#include <cstdlib>   // For rand() function
using namespace std;

// Transaction class to represent each transaction
class Transaction {
private:
    int id;
    double amount;
    string date;

public:
    Transaction(int id, double amount, string date) : id(id), amount(amount), date(date) {}

    int getId() const { return id; }
    double getAmount() const { return amount; }
    string getDate() const { return date; }

    void print() const {
        cout << "Transaction ID: " << id << ", Amount: " << amount << ", Date: " << date << endl;
    }
};

// Function to generate a random transaction
Transaction generateRandomTransaction() {
    srand(static_cast<unsigned>(time(0)));  // Seed the random number generator

    int id = rand() % 1000 + 1;  // Generate a random ID between 1 and 999
    double amount = static_cast<double>(rand()) / 100.0;  // Generate a random amount between 0.01 and 99.99
    // Generate a random date in the format "YYYY-MM-DD"
    const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year = rand() % 20 + 2020;
    int month = rand() % 12 + 1;
    int day = rand() % days_in_month[month - 1] + 1;
    string date = to_string(year) + "-" + to_string(month) + "-" + to_string(day);

    return Transaction(id, amount, date);
}

int main() {
    int numTransactions;
    cout << "Enter the number of transactions you want to generate: ";
    cin >> numTransactions;

    if (numTransactions <= 0) {
        cout << "Invalid number of transactions. Please enter a positive value." << endl;
        return 1;
    }

    // Use a std::list to store transactions
    list<Transaction> transactions;

    // Generate random transactions and add them to the list
    for (int i = 0; i < numTransactions; i++) {
        Transaction randomTransaction = generateRandomTransaction();
        transactions.push_back(randomTransaction);
    }

    cout << "\nGenerated Transactions: \n";
    for (const Transaction& transaction : transactions) {
        transaction.print();
    }

    // Optional: You can add more features to manage the transactions here

    cout << "\nTransaction list management complete." << endl;
    return 0;
}

