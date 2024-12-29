
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
    std::cout << "Transactions:\n";    std::cout << "ID\tAmount\n";    for (const auto& transaction : transactions) {        std::cout << transaction.id << "\t" << transaction.amount << "\n";    }
}

void benchmarkAndCompare(const std::vector<Transaction>& vec, const std::deque<Transaction>& deque, const std::string& containerName) {
    auto startVector = std::chrono::high_resolution_clock::now();

    // Retrieve from front
    auto frontVector = vec.front();

    auto endVector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> frontDurationVector = endVector - startVector;

    startVector = std::chrono::high_resolution_clock::now();
    // Retrieve from back 
    auto backVector = vec.back();
    endVector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> backDurationVector = endVector - startVector;
    
    int randomIndex = rand() % vec.size();

    startVector = std::chrono::high_resolution_clock::now();

    // Retrieve from a random position 
    auto randomTransactionVector = vec[randomIndex]; 

    endVector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> randomDurationVector = endVector - startVector;

    auto startDeque = std::chrono::high_resolution_clock::now();
    // Retrieve from front 
    auto frontDeque = deque.front(); 
    auto endDeque = std::chrono::high_resolution_clock::now();    std::chrono::duration<double, std::milli> frontDurationDeque = endDeque - startDeque;

    startDeque = std::chrono::high_resolution_clock::now();
    // Retrieve from back
    auto backDeque = deque.back();    
    endDeque = std::chrono::high_resolution_clock::now();    
    std::chrono::duration<double, std::milli> backDurationDeque = endDeque - startDeque;
   
    startDeque = std::chrono::high_resolution_clock::now(); 

    // Retrieve from a random position
    auto randomTransactionDeque = deque[randomIndex];  
    endDeque = std::chrono::high_resolution_clock::now();    
    std::chrono::duration<double, std::milli> randomDurationDeque = endDeque - startDeque;

    std::cout << "Vector vs. Deque Retrieval Times for " << containerName << "(in ms):\n";
    std::cout << "Front: Vector - " << frontDurationVector.count() << " ms, Deque - " << frontDurationDeque.count(); 
    std::cout << (frontDurationVector < frontDurationDeque ? ", Vector" : ", Deque") << " performs better.\n"; 
    std::cout << "Back: Vector - " << backDurationVector.count() << " ms, Deque - " << backDurationDeque.count(); 
    std::cout << (backDurationVector < backDurationDeque ? ", Vector" : ", Deque") << " performs better.\n";
    std::cout << "Random: Vector - " << randomDurationVector.count() << " ms, Deque - " << randomDurationDeque.count(); 
    std::cout << (randomDurationVector < randomDurationDeque ? ", Vector" : ", Deque") << " performs better.\n";