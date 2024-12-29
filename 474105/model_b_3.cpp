
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

vector<int> iterateSignalMultiplicationArithmetic(int signal, int n)
{
    vector<int> result;

    if (n <= 0)
    {
        return result;
    }

    result.push_back(signal);

    for (int i = 1; i <= n; i++)
    {
        signal *= 2;
        result.push_back(signal);
    }

    return result;
}

vector<int> iterateSignalMultiplicationBitwise(int signal, int n)
{
    vector<int> result;

    if (n <= 0)
    {
        return result;
    }

    result.push_back(signal);

    for (int i = 1; i <= n; i++)
    {
        signal <<= 1;
        result.push_back(signal);
    }

    return result;
}

int main() {
    int signal = 10000; // Choose a larger signal value for meaningful benchmarking
    int n = 20;         // Number of iterations

    // Benchmark using Arithmetic Multiplication
    {
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = iterateSignalMultiplicationArithmetic(signal, n);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        cout << "Arithmetic Multiplication: " << duration.count() << " nanoseconds\n";
    }

   // Benchmark using Bitwise Shift Operator
    {
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = iterateSignalMultiplicationBitwise(signal, n);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        cout << "Bitwise Shift Operator: " << duration.count() << " nanoseconds\n";
    }

    return 0;
}
