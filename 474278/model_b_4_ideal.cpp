#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct Transaction {
    int id;
    double amount;

    // Default constructor
    Transaction() : id(0), amount(0.0) {}

    // Parameterized constructor
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

void benchmarkArrayRetrieval(const std::array<Transaction, 10000>& transactions, double& frontTime, double& backTime, double& randomTime) {
    auto start = std::chrono::high_resolution_clock::now();
    auto front = transactions.front();
    auto end = std::chrono::high_resolution_clock::now();
    frontTime = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto back = transactions.back();
    end = std::chrono::high_resolution_clock::now();
    backTime = std::chrono::duration<double, std::milli>(end - start).count();

    // Retrieve from a random position (direct access by index)
    int randomIndex = rand() % transactions.size();
    start = std::chrono::high_resolution_clock::now();
    auto randomTransaction = transactions[randomIndex];
    end = std::chrono::high_resolution_clock::now();
    randomTime = std::chrono::duration<double, std::milli>(end - start).count();
}

void comparePerformance(double vectorFront, double arrayFront, 
                        double vectorBack, double arrayBack, 
                        double vectorRandom, double arrayRandom) {
    std::cout << "Performance Comparison:\n";
    std::cout << "Front Access: " << (vectorFront < arrayFront ? "Vector is faster" : "Array is faster") << "\n";
    std::cout << "Back Access: " << (vectorBack < arrayBack ? "Vector is faster" : "Array is faster") << "\n";
    std::cout << "Random Access: " << (vectorRandom < arrayRandom ? "Vector is faster" : "Array is faster") << "\n";
}

int main() {
    const int numberOfTransactions = 10000;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random transactions and add them to the vector and array
    std::vector<Transaction> vectorTransactions;
    std::array<Transaction, numberOfTransactions> arrayTransactions;

    for (int i = 1; i <= numberOfTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100; // Random amount between 0.00 and 99.99
        Transaction transaction(i, randomAmount);
        vectorTransactions.push_back(transaction);
        arrayTransactions[i - 1] = transaction;
    }

    // Variables to hold the time taken for each retrieval
    double vectorFrontTime, vectorBackTime, vectorRandomTime;
    double arrayFrontTime, arrayBackTime, arrayRandomTime;

    // Benchmark vector retrieval
    benchmarkVectorRetrieval(vectorTransactions, vectorFrontTime, vectorBackTime, vectorRandomTime);

    // Benchmark array retrieval
    benchmarkArrayRetrieval(arrayTransactions, arrayFrontTime, arrayBackTime, arrayRandomTime);

    // Print out the times
    std::cout << "Vector Retrieval Times (in ms):\n";
    std::cout << "Front: " << vectorFrontTime << " ms\n";
    std::cout << "Back: " << vectorBackTime << " ms\n";
    std::cout << "Random: " << vectorRandomTime << " ms\n";

    std::cout << "\nArray Retrieval Times (in ms):\n";
    std::cout << "Front: " << arrayFrontTime << " ms\n";
    std::cout << "Back: " << arrayBackTime << " ms\n";
    std::cout << "Random: " << arrayRandomTime << " ms\n";

    // Compare performance
    comparePerformance(vectorFrontTime, arrayFrontTime,
                       vectorBackTime, arrayBackTime,
                       vectorRandomTime, arrayRandomTime);

    return 0;
}
