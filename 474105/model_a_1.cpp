#include <iostream>
#include <vector>

int main() {
    // Initial signal value
    double signal = 1.0; // You can set this to any initial value
    // Threshold value
    double n;

    // Prompt user to enter the threshold value
    std::cout << "Enter the threshold value (n): ";
    std::cin >> n;

    // Vector to store the resulting values
    std::vector<double> results;

    // Iteratively multiply the signal by 2 until it reaches or exceeds n
    while (signal < n) {
        results.push_back(signal);
        signal *= 2; // Multiply the signal by 2
    }

    // Print the resulting values
    std::cout << "Resulting values:" << std::endl;
    for (const auto& value : results) {
        std::cout << value << std::endl;
    }

    return 0;
}