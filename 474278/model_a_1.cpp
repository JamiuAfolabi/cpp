#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

struct Transaction {
    int id;
    double amount;

    Transaction(int i, double a) : id(i), amount(a) {}
};

void displayTransactions(const std::list<Transaction>& transactions) {
    std::cout << "Transactions:\n";
    std::cout << "ID\tAmount\n";
    for (const auto& transaction : transactions) {
        std::cout << transaction.id << "\t" << transaction.amount << "\n";
    }
}

int main() {
    std::list<Transaction> transactions;
    int numberOfTransactions;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "Enter the number of transactions to generate: ";
    std::cin >> numberOfTransactions;

    // Validate input
    if (numberOfTransactions <= 0) {
        std::cout << "Number of transactions must be positive.\n";
        return 1;
    }

    // Generate random transactions and add them to the list
    for (int i = 1; i <= numberOfTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100; // Random amount between 0.00 and 99.99
        transactions.emplace_back(i, randomAmount);
    }

    // Display the transactions
    displayTransactions(transactions);

    return 0;
}