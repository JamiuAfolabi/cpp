#include <iostream>
#include <vector>
#include <cmath>
#include <cstdarg>

// Set the default Z-score threshold using a preprocessor directive
#ifndef Z_SCORE_THRESHOLD
#define Z_SCORE_THRESHOLD 2.0
#endif

// Define a safer version of the macro without GNU-specific extensions
#define LOG_FUNCTION_ENTRY(fmt, ...) \
    do { \
        printf("Entering function: %s(", __FUNCTION__); \
        printf(fmt, __VA_ARGS__); \
        printf(")\n"); \
    } while (0)

#define LOG_FUNCTION_EXIT() \
    do { \
        printf("Exiting function: %s\n", __FUNCTION__); \
    } while (0)

double calculateMean(const std::vector<double>& values) {
    LOG_FUNCTION_ENTRY("const std::vector<double>& values.size() = %zu", values.size());

    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }

    LOG_FUNCTION_EXIT();
    return sum / values.size();
}

double calculateStandardDeviation(const std::vector<double>& values, double mean) {
    LOG_FUNCTION_ENTRY("const std::vector<double>& values.size() = %zu, double mean = %.2lf", values.size(), mean);

    double sum = 0.0;
    for (double value : values) {
        sum += (value - mean) * (value - mean);
    }

    LOG_FUNCTION_EXIT();
    return std::sqrt(sum / values.size());
}

void findOutliers(const std::vector<double>& values, double threshold) {
    LOG_FUNCTION_ENTRY("const std::vector<double>& values.size() = %zu, double threshold = %.2lf", values.size(), threshold);
    
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
    double zScoreThreshold = Z_SCORE_THRESHOLD; // Set to the threshold define

    findOutliers(values, zScoreThreshold);

    return 0;
}
