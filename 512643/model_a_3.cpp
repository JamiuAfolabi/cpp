#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <chrono>
#include <random>

const int NUM_RECORDS = 1000;

void benchmarkVector() {
    std::vector<int> vec;
    std::chrono::high_resolution_clock::time_point start, end;

    // Insertion at the back
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RECORDS; ++i) {
        vec.push_back(i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Vector insertion at back took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // Insertion at the front
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RECORDS; ++i) {
        vec.insert(vec.begin(), i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Vector insertion at front took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // Searching for an element
    start = std::chrono::high_resolution_clock::now();
    int searchValue = NUM_RECORDS / 2; // Search for the middle element
    auto it = std::find(vec.begin(), vec.end(), searchValue);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Vector searching took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

void benchmarkDeque() {
    std::deque<int> deq;
    std::chrono::high_resolution_clock::time_point start, end;

    // Insertion at the back
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RECORDS; ++i) {
        deq.push_back(i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deque insertion at back took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // Insertion at the front
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_RECORDS; ++i) {
        deq.push_front(i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deque insertion at front took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // Searching for an element
    start = std::chrono::high_resolution_clock::now();
    int searchValue = NUM_RECORDS / 2; // Search for the middle element
    auto it = std::find(deq.begin(), deq.end(), searchValue);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deque searching took "
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

int main() {
    std::cout << "Benchmarking std::vector:\n";
    benchmarkVector();

    std::cout << "\nBenchmarking std::deque:\n";
    benchmarkDeque();

    return 0;
}