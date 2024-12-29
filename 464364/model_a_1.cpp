#include <iostream>

// Inline function to calculate the square root
inline double mySqrt(double number) {
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
    } while (abs(result - guess) > epsilon); // Loop until within precision

    return guess;
}

int main() {
    double number = 25.0;
    double result = mySqrt(number);
    std::cout << "The square root of " << number << " is approximately " << result << std::endl;
    return 0;
}