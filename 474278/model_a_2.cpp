#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct Transaction {
    int id;
    double amount;

    Transaction(int i, double a) : id(i), amount(a) {}
};

void displayTransactions(const std::vector<Transaction>& transactions) {
    std::cout << "Transactions:\n";
    std::cout << "ID\tAmount\n";
    for (const auto& transaction : transactions) {
        std::cout << transaction.id << "\t" << transaction.amount << "\n";
    }
}

void benchmarkVectorRetrieval(const std::vector<Transaction>& transactions) {
    auto start = std::chrono::high_resolution_clock::now();
    // Retrieve from front
    auto front = transactions.front();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> frontDuration = end - start;

    start = std::chrono::high_resolution_clock::now();
    // Retrieve from back
    auto back = transactions.back();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> backDuration = end - start;

    int randomIndex = rand() % transactions.size();
    start = std::chrono::high_resolution_clock::now();
    // Retrieve from a random position
    auto randomTransaction = transactions[randomIndex];
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> randomDuration = end - start;

    std::cout << "Vector Retrieval Times (in ms):\n";
    std::cout << "Front: " << frontDuration.count() << " ms\n";
    std::cout << "Back: " << backDuration.count() << " ms\n";
    std::cout << "Random: " << randomDuration.count() << " ms\n";
}

void benchmarkListRetrieval(const std::list<Transaction>& transactions) {
    auto start = std::chrono::high_resolution_clock::now();
    // Retrieve from front
    auto front = transactions.front();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> frontDuration = end - start;

    start = std::chrono::high_resolution_clock::now();
    // Retrieve from back
    auto back = transactions.back();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> backDuration = end - start;

    // Retrieve from a random position
    start = std::chrono::high_resolution_clock::now();
    auto it = transactions.begin();
    std::advance(it, rand() % transactions.size());
    auto randomTransaction = *it;
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> randomDuration = end - start;

    std::cout << "List Retrieval Times (in ms):\n";
    std::cout << "Front: " << frontDuration.count() << " ms\n";
    std::cout << "Back: " << backDuration.count() << " ms\n";
    std::cout << "Random: " << randomDuration.count() << " ms\n";
}

int main() {
    std::vector<Transaction> vectorTransactions;
    std::list<Transaction> listTransactions;
    int numberOfTransactions = 10000;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random transactions and add them to both the vector and list
    for (int i = 1; i <= numberOfTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100; // Random amount between 0.00 and 99.99
        Transaction transaction(i, randomAmount);
        vectorTransactions.push_back(transaction);
        listTransactions.push_back(transaction);
    }

    // Benchmark vector retrieval
    benchmarkVectorRetrieval(vectorTransactions);

    // Benchmark list retrieval
    benchmarkListRetrieval(listTransactions);

    return 0;
}