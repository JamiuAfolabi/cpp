#include <iostream>
#include <array>
#include <vector>
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
 
template <class Container>
void benchmarkContainerRetrieval(const Container& container, double& frontTime, double& backTime, double& randomTime) {
    auto start = std::chrono::high_resolution_clock::now();
    auto front = container.front();
    auto end = std::chrono::high_resolution_clock::now();
    frontTime = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto back = container.back();
    end = std::chrono::high_resolution_clock::now();
    backTime = std::chrono::duration<double, std::milli>(end - start).count();

    int randomIndex = static_cast<int>(std::rand() % container.size());
    start = std::chrono::high_resolution_clock::now();
    auto randomTransaction = container[randomIndex];
    end = std::chrono::high_resolution_clock::now();
    randomTime = std::chrono::duration<double, std::milli>(end - start).count();
}

void comparePerformance(double vectorFront, double arrayFront,
                        double vectorBack, double arrayBack,
                        double vectorRandom, double arrayRandom) {
    std::cout << "\nPerformance Comparison:\n";
    std::cout << "Front Access: " << ((vectorFront < arrayFront) ? "Vector is faster" : "Array is faster") << "\n";
    std::cout << "Back Access: " << ((vectorBack < arrayBack) ? "Vector is faster" : "Array is faster") << "\n";
    std::cout << "Random Access: " << ((vectorRandom < arrayRandom) ? "Vector is faster" : "Array is faster") << "\n";
}
 
int main() {
    const int numberOfTransactions = 10000;
    std::vector<Transaction> vectorTransactions;
    
    // We can't use std::array directly with 10000 elements as it requires static compiler-known size
    // So we'll compare with a vector of fixed size
    std::array<Transaction, numberOfTransactions> arrayTransactions; 
 
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    double vectorFrontTime, arrayFrontTime;
    double vectorBackTime, arrayBackTime;
    double vectorRandomTime, arrayRandomTime;
 
    // Generate random transactions and add them to vector
    for (int i = 1; i <= numberOfTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100.0;
        vectorTransactions.push_back({i, randomAmount});
        arrayTransactions[i - 1] = {i, randomAmount};
    }
 
    benchmarkContainerRetrieval(vectorTransactions, vectorFrontTime, vectorBackTime, vectorRandomTime);
    benchmarkContainerRetrieval(arrayTransactions, arrayFrontTime, arrayBackTime, arrayRandomTime);