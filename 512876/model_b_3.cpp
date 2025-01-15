#include <iostream>
#include <vector>
#include <cmath>
#include <cstdarg>
#include <string>

#define Z_SCORE_THRESHOLD 2.0

#define LOG_FUNCTION_ENTRY(fmt, ...) \
    do { \
        fprintf(stderr, "Entering function: %s", __FUNCTION__); \
        fprintf(stderr, "(" fmt ")\n", __VA_ARGS__); \
    } while (0)

#define LOG_FUNCTION_EXIT() \
    do { \
        fprintf(stderr, "Exiting function: %s\n", __FUNCTION__); \
    } while (0)

double calculateMean(const std::vector<double>& values) {
    LOG_FUNCTION_ENTRY("const std::vector<double>* values = %p", &values);

    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }

    LOG_FUNCTION_EXIT();
    return sum / values.size();
}

double calculateStandardDeviation(const std::vector<double>& values, double mean) {
    LOG_FUNCTION_ENTRY("const std::vector<double>* values = %p, double mean = %.1lf", &values, mean);

    double sum = 0.0;
    for (double value : values) {
        sum += (value - mean) * (value - mean);
    }

    LOG_FUNCTION_EXIT();
    return std::sqrt(sum / values.size());
}

void findOutliers(const std::vector<double>& values, double threshold = Z_SCORE_THRESHOLD) {
    LOG_FUNCTION_ENTRY("const std::vector<double>* values = %p, double threshold = %.1lf", &values, threshold);

    double mean = calculateMean(values);
    double stddev = calculateStandardDeviation(values, mean);

    std::cout << "Mean: " << mean << "\n";
    std::cout << "Standard Deviation: " << stddev << "\n";
    std::cout << "Outliers:\n";

    for (double value : values) {
        double zScore = (value - mean) / stddev;
        if (std::abs(zScore) > threshold) {
            std::cout << value << " (Z-score: " << zScore << ")\n";
        }
    }

    LOG_FUNCTION_EXIT();
}

int main() {
    std::vector<double> values = {10, 12, 12, 13, 12, 12, 14, 15, 16, 100}; // Sample data
    findOutliers(values);
    
    return 0;
} 