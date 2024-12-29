#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>

struct Transaction {
    int id;
    double amount;

    Transaction(int i, double a) : id(i), amount(a) {}
};
using VectorTransaction = std::vector<Transaction>;
using ListTransaction = std::list<Transaction>;

 template <typename Container>
 bool findTransaction(const Container& transactions, int transactionID, auto& foundTime) {
  auto start = std::chrono::high_resolution_clock::now();
   using Time = decltype(start);

   bool found = false;
   for (const auto& transaction : transactions) {
    if (transaction.id == transactionID) {
        found = true;
        break;
    }
   }

   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
   foundTime = duration.count();

   return found;
 }

  template <typename Container>
 void findTransactionAtFront(const Container& transactions, int numTransactions, auto& foundTime) {
     findTransaction(transactions, 1, foundTime);
 }

 template <typename Container>
 void findTransactionAtBack(const Container& transactions, int numTransactions, auto& foundTime) {
     findTransaction(transactions, numTransactions, foundTime);
 }

 template <typename Container>
 void findTransactionAtRandomPosition(const Container& transactions, int numTransactions, auto& foundTime) {
  int randomIndex = std::rand() % numTransactions + 1;
  findTransaction(transactions, randomIndex, foundTime);
 }

 void displayTransactions(const VectorTransaction& transactions) {
    std::cout << "\nVector Transactions:\n";
    std::cout << "ID\tAmount\n";
    for (const auto& transaction : transactions) {
        std::cout << transaction.id << "\t" << transaction.amount << "\n";
    }
}
 void displayTransactions(const ListTransaction& transactions) {
    std::cout << "\nList Transactions:\n";
    std::cout << "ID\tAmount\n";
    for (const auto& transaction : transactions) {
        std::cout << transaction.id << "\t" << transaction.amount << "\n";
    }
}
int main() {
    int numTransactions = 10000;

    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(0)));

    VectorTransaction vectorTransactions;
    ListTransaction listTransactions;
    
    // Generate random transactions
    for (int i = 1; i <= numTransactions; ++i) {
        double randomAmount = static_cast<double>(std::rand() % 10000) / 100;
        vectorTransactions.push_back(Transaction(i, randomAmount));
        listTransactions.emplace_back(Transaction(i, randomAmount));
    }
     double vectortimeFront, vectortimeBack, vectortimeRandom;
     double listtimeFront, listtimeBack, listtimeRandom;

      std::cout << "Performance Benchmark using Vector:\n";
    
     std::cout << "-- Front Transaction:\n";
     findTransactionAtFront(vectorTransactions, numTransactions, vectortimeFront);
     std::cout << "Time taken: " << vectortimeFront << " seconds" << std::endl;

      std::cout << "\n-- Back Transaction:\n";
     findTransactionAtBack(vectorTransactions, numTransactions, vectortimeBack);
     std::cout << "Time taken: " << vectortimeBack << " seconds" << std::endl;