#include <iostream>
#include <chrono>
#include <cmath> // For std::sqrt

// Non-inline function to calculate the square root
double mySqrt(double number) {
    if (number < 0) {
        std::cerr << "Error: Negative input to sqrt." << std::endl;
        return -1; // Handle error for negative input
    }
    double guess = number / 2.0; // Initial guess
    double epsilon = 0.0001; // Precision
    double result;

    // Newton's method for square root approximation
    do {
        result = guess;
        guess = (result + number / result) / 2.0;
    } while (std::abs(result - guess) > epsilon); // Loop until within precision

    return guess;
}

// Inline function to calculate the square root
inline double mySqrtInline(double number) {
    if (number < 0) {
        std::cerr << "Error: Negative input to sqrt." << std::endl;
        return -1; // Handle error for negative input
    }
    double guess = number / 2.0; // Initial guess
    double epsilon = 0.0001; // Precision
    double result;

    // Newton's method for square root approximation
    do {
        result = guess;
        guess = (result + number / result) / 2.0;
    } while (std::abs(result - guess) > epsilon); // Loop until within precision

    return guess;
}

int main() {
    const int iterations = 1000000; // Number of iterations to benchmark
    double number = 25.0; // Number to calculate the square root of

    // Benchmarking non-inline function
    auto startNonInline = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        mySqrt(number);
    }
    auto endNonInline = std::chrono::high_resolution_clock::now();
    auto durationNonInline = std::chrono::duration_cast<std::chrono::microseconds>(endNonInline - startNonInline).count();

    // Benchmarking inline function
    auto startInline = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        mySqrtInline(number);
    }
    auto endInline = std::chrono::high_resolution_clock::now();
    auto durationInline = std::chrono::duration_cast<std::chrono::microseconds>(endInline - startInline).count();

    // Output the results
    std::cout << "Non-inline function execution time: " << durationNonInline << " microseconds" << std::endl;
    std::cout << "Inline function execution time: " << durationInline << " microseconds" << std::endl;

    return 0;
}