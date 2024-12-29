
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Function to iterate signal multiplication using standard arithmetic
template<typename T>
vector<T> iterateSignalMultiplicationArithmetic(T signal, int n) {
    vector<T> result;
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

// Function to iterate signal multiplication using bitwise shift (for integer signals)
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
    double signal;  
    int n;

    cout << "Enter the signal value: ";
    cin >> signal;

    cout << "Enter the value of n: ";
    cin >> n;

    if(signal == static_cast<int>(signal)) {
        // If the signal is an integer, use the bitwise function
        cout << "\nSignal is an integer. Using bitwise operations..." << endl;

        // Benchmarking and calling the integer function
        auto startBitwise = chrono::high_resolution_clock::now();
        vector<int> resultBitwise = iterateSignalMultiplicationBitwise(static_cast<int>(signal), n);
        auto endBitwise = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> durationBitwise = endBitwise - startBitwise;

        cout << "Results with bitwise shift multiplication:\n";
        for (int val : resultBitwise) {
            cout << val << " ";
        }
        cout << "\nTime taken (Bitwise): " << durationBitwise.count() << " ms\n";

    } else {
        // If the signal is a floating-point number, use the arithmetic function
        cout << "\nSignal is a floating-point number. Using arithmetic operations..." << endl;
        // Benchmarking and calling the floating-point function
        auto startArithmetic = chrono::high_resolution_clock::now();
        vector<double> resultArithmetic = iterateSignalMultiplicationArithmetic(signal, n);
        auto endArithmetic = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> durationArithmetic = endArithmetic - startArithmetic;

        cout << "Results with standard arithmetic multiplication:\n";
        for (double val : resultArithmetic) {
            cout << val << " ";
        }
        cout << "\nTime taken (Arithmetic): " << durationArithmetic.count() << " ms\n";
    }

    return 0;
}
