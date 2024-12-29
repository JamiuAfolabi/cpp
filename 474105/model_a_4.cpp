#include <iostream>
#include <vector>
#include <chrono>
#include <type_traits>

using namespace std;

// Template function to iterate signal multiplication
template <typename T>
vector<T> iterateSignalMultiplication(T signal, int n) {
    static_assert(std::is_arithmetic<T>::value, "Template parameter must be a numeric type");

    vector<T> result;

    if (n <= 0) {
        return result;
    }

    result.push_back(signal);

    for (int i = 1; i <= n; i++) {
        if (std::is_integral<T>::value) {
            // Use bitwise shift for integers
            signal = signal << 1;  // Bitwise shift
        } else {
            // Use standard multiplication for floating-point numbers
            signal *= 2;  // Standard multiplication
        }
        result.push_back(signal);
    }

    return result;
}

int main() {
    double floatSignal;
    int intSignal;
    int n;
    char choice;

    cout << "Enter 'i' for integer or 'f' for floating-point signal: ";
    cin >> choice;

    if (choice == 'i') {
        cout << "Enter the integer signal value: ";
        cin >> intSignal;

        cout << "Enter the value of n: ";
        cin >> n;

        // Benchmarking bitwise shift multiplication
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = iterateSignalMultiplication(intSignal, n);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> duration = end - start;

        cout << "\nResults with bitwise shift multiplication:\n";
        for (int val : result) {
            cout << val << " ";
        }
        cout << "\nTime taken: " << duration.count() << " ms\n";

    } else if (choice == 'f') {
        cout << "Enter the floating-point signal value: ";
        cin >> floatSignal;

        cout << "Enter the value of n: ";
        cin >> n;

        // Benchmarking standard arithmetic multiplication
        auto start = chrono::high_resolution_clock::now();
        vector<double> result = iterateSignalMultiplication(floatSignal, n);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> duration = end - start;

        cout << "\nResults with standard arithmetic multiplication:\n";
        for (double val : result) {
            cout << val << " ";
        }
        cout << "\nTime taken: " << duration.count() << " ms\n";

    } else {
        cout << "Invalid choice! Please enter 'i' for integer or 'f' for floating-point." << endl;
    }

    return 0;
}