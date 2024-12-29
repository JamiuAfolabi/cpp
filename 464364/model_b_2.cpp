#include <iostream>
#include <chrono>
#include <functional>


// Make sure we cache the std::sqrt within the benchmark scope
// to avoid doing extra memory indirections via global functions.
const auto sqrtFunction = static_cast<std::function<double(double)>>(&std::sqrt);


double mySqrt_inline(double number);
double mySqrt_not_inline(double number);

int main() {
    const double number = 25.0;
    int num_iterations = 10000000; // Make sure the iterations are enough for measurable timing

    {
        // Benchmark for mySqrt_inline
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iterations; i++) {
            mySqrt_inline(number);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "mySqrt_inline " << num_iterations << " calls took: " << duration.count() << " ns\n";
    }

    {
        // Benchmark for mySqrt_not_inline
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iterations; i++) {
            mySqrt_not_inline(number);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "mySqrt_not_inline " << num_iterations << " calls took: " << duration.count() << " ns\n";
    }
    
    {
        // Benchmark for std::sqrt
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iterations; i++) {
            sqrtFunction(number);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "std::sqrt " << num_iterations << " calls took: " << duration.count() << " ns\n";
    }


    return 0;
}

// Function definition for inlined square root function
double mySqrt_inline(double number) {
    if (number < 0) {  // Error handling for negative input
        return -1.0;
    }

    double x = number / 2.0;
    double epsilon = 0.0001;

    while (std::abs(x * x - number) > epsilon) {
        x = 0.5 * (x + number / x);
    }
    return x;
}

// Function definition for NOT inlined square root function
double mySqrt_not_inline(double number) {
    return mySqrtHelper(number);
}

double mySqrtHelper(double number) {
    if (number < 0) {  // Error handling for negative input
        return -1.0;
    }

    double x = number / 2.0;
    double epsilon = 0.0001;

    while (std::abs(x * x - number) > epsilon) {
        x = 0.5 * (x + number / x);
    }
    return x;
}