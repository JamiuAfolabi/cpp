#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

template <typename Container>
void benchmark(Container& container) {
    // Insertion at front
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        container.push_front(i);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken for " << typeid(container).name() << ": Inserting 1000 elements at front: " << duration.count() << " microseconds\n";

    // Insertion at back
    start = chrono::high_resolution_clock::now();
    for (int i = 1000; i < 2000; ++i) {
        container.push_back(i);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken for " << typeid(container).name() << ": Inserting 1000 elements at back: " << duration.count() << " microseconds\n";

    // Search for an element
    int searchValue = 1999;
    start = chrono::high_resolution_clock::now();
    auto it = find(container.begin(), container.end(), searchValue);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken for " << typeid(container).name() << ": Searching for element " << searchValue << ": " << duration.count() << " microseconds\n";

    cout << "---------------------------------------\n";
}

int main() {
    cout << "Benchmarking std::vector:\n";
    vector<int> vec;
    benchmark(vec);

    cout << "\nBenchmarking std::deque:\n";
    deque<int> deq;
    benchmark(deq);

    return 0;
}