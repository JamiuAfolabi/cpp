#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Function to iterate signal multiplication using standard arithmetic
vector<int> iterateSignalMultiplicationArithmetic(int signal, int n) {
    vector<int> result;
    if (n <= 0) {
        return result;
    }
    result.push_back(signal);
    for (int i = 1; i <= n; i++) {
        signal *= 2;  // Standard multiplication
        result.push_back(signal);
    }
    return result;
}

// Function to iterate signal multiplication using bitwise shift
vector<int> iterateSignalMultiplicationBitwise(int signal, int n) {
    vector<int> result;
    if (n <= 0) {
        return result;
    }
    result.push_back(signal);
    for (int i = 1; i <= n; i++) {
        signal = signal << 1;  // Bitwise shift
        result.push_back(signal);
    }
    return result;
}

int main() {
    int signal, n;

    cout << "Enter the signal value (integer): ";
    cin >> signal;

    cout << "Enter the value of n: ";
    cin >> n;

    // Benchmarking standard arithmetic multiplication
    auto startArithmetic = chrono::high_resolution_clock::now();
    vector<int> resultArithmetic = iterateSignalMultiplicationArithmetic(signal, n);
    auto endArithmetic = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> durationArithmetic = endArithmetic - startArithmetic;

    // Benchmarking bitwise shift multiplication
    auto startBitwise = chrono::high_resolution_clock::now();
    vector<int> resultBitwise = iterateSignalMultiplicationBitwise(signal, n);
    auto endBitwise = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> durationBitwise = endBitwise - startBitwise;

    // Output results
    cout << "\nResults with standard arithmetic multiplication:\n";
    for (int val : resultArithmetic) {
        cout << val << " ";
    }
    cout << "\nTime taken (Arithmetic): " << durationArithmetic.count() << " ms\n";

    cout << "\nResults with bitwise shift multiplication:\n";
    for (int val : resultBitwise) {
        cout << val << " ";
    }
    cout << "\nTime taken (Bitwise): " << durationBitwise.count() << " ms\n";

    return 0;
}