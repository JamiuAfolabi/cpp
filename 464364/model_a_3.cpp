#include <iostream>
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

    // Call the non-inline function
    for (int i = 0; i < iterations; ++i) {
        mySqrt(number);
    }

    // Call the inline function
    for (int i = 0; i < iterations; ++i) {
        mySqrtInline(number);
    }

    return 0;
}