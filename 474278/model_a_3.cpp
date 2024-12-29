#include <iostream>
#include <vector>
#include <deque>
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

void benchmarkVectorRetrieval(const std::vector<Transaction>& transactions, double& frontTime, double& backTime, double& randomTime) {
    auto start = std::chrono::high_resolution_clock::now();
    auto front = transactions.front();
    auto end = std::chrono::high_resolution_clock::now();
    frontTime = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto back = transactions.back();
    end = std::chrono::high_resolution_clock::now();
    backTime = std::chrono::duration<double, std::milli>(end - start).count();

    int randomIndex = rand() % transactions.size();
    start = std::chrono::high_resolution_clock::now();
    auto randomTransaction = transactions[randomIndex];
    end = std::chrono::high_resolution_clock::now();
    randomTime = std::chrono::duration<double, std::milli>(end - start).count();
}

void benchmarkDequeRetrieval(const std::deque<Transaction>& transactions, double& frontTime, double& backTime, double& randomTime) {
    auto start = std::chrono::high_resolution_clock::now();
    auto front = transactions.front();
    auto end = std::chrono::high_resolution_clock::now();
    frontTime = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto back = transactions.back();
    end = std::chrono::high_resolution_clock::now();
    backTime = std::chrono::duration<double, std::milli>(end - start).count();

    // Retrieve from a random position
    start = std::chrono::high_resolution_clock::now();
    auto it = transactions.begin();
    std::advance(it, rand() % transactions.size());
    auto randomTransaction = *it;
    end = std::chrono::high_resolution_clock::now();
    randomTime = std::chrono::duration<double, std::milli>(end - start).count();
}

void comparePerformance(double vectorFront, double dequeFront, 
                        double vectorBack, double dequeBack, 
                        double vectorRandom, double dequeRandom) {
    std::cout << "Performance Comparison:\n";
    std::cout << "Front Access: " << (vectorFront < dequeFront ? "Vector is faster" : "Deque is faster") << "\n";
    std::cout << "Back Access: " << (vectorBack < dequeBack ? "Vector is faster" : "Deque is faster") << "\n";
    std::cout << "Random Access: " << (vectorRandom < dequeRandom ? "Vector is faster" : "Deque is faster") << "\n";
}

int main() {
    std::vector<Transaction> vectorTransactions;
    std::deque<Transaction> dequeTransactions;
    int numberOfTransactions = 10000;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random transactions and add them to both the vector and deque
    for (int i = 1; i <= numberOfTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100; // Random amount between 0.00 and 99.99
        Transaction transaction(i, randomAmount);
        vectorTransactions.push_back(transaction);
        dequeTransactions.push_back(transaction);
    }

    // Variables to hold the time taken for each retrieval
    double vectorFrontTime, vectorBackTime, vectorRandomTime;
    double dequeFrontTime, dequeBackTime, dequeRandomTime;

    // Benchmark vector retrieval
    benchmarkVectorRetrieval(vectorTransactions, vectorFrontTime, vectorBackTime, vectorRandomTime);

    // Benchmark deque retrieval
    benchmarkDequeRetrieval(dequeTransactions, dequeFrontTime, dequeBackTime, dequeRandomTime);

    // Print out the times
    std::cout << "Vector Retrieval Times (in ms):\n";
    std::cout << "Front: " << vectorFrontTime << " ms\n";
    std::cout << "Back: " << vectorBackTime << " ms\n";
    std::cout << "Random: " << vectorRandomTime << " ms\n";

    std::cout << "\nDeque Retrieval Times (in ms):\n";
    std::cout << "Front: " << dequeFrontTime << " ms\n";
    std::cout << "Back: " << dequeBackTime << " ms\n";
    std::cout << "Random: " << dequeRandomTime << " ms\n";

    // Compare performance
    comparePerformance(vectorFrontTime, dequeFrontTime,
                       vectorBackTime, dequeBackTime,
                       vectorRandomTime, dequeRandomTime);

    return 0;
}