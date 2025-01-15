#include <iostream>
#include <vector>
#include <cmath>

// Macro for logging function entry and exit
#define LOG_FUNCTION_ENTRY(func, ...) { \
    std::cout << "Entering function: " << #func << " with parameters: "; \
    logParams(__VA_ARGS__); \
}

#define LOG_FUNCTION_EXIT(func) { \
    std::cout << "Exiting function: " << #func << "\n"; \
}

// Function to log parameters
void logParams() {
    std::cout << "\n";
}

template<typename T, typename... Args>
void logParams(T first, Args... args) {
    std::cout << first << " ";
    logParams(args...);
}

double calculateMean(const std::vector<double>& values) {
    LOG_FUNCTION_ENTRY(calculateMean, values.size());
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    
    double mean = sum / values.size();
    LOG_FUNCTION_EXIT(calculateMean);
    return mean;
}

double calculateStandardDeviation(const std::vector<double>& values, double mean) {
    LOG_FUNCTION_ENTRY(calculateStandardDeviation, values.size(), mean);
    double sum = 0.0;
    for (double value : values) {
        sum += (value - mean) * (value - mean);
    }
    
    double stddev = std::sqrt(sum / values.size());
    LOG_FUNCTION_EXIT(calculateStandardDeviation);
    return stddev;
}

void findOutliers(const std::vector<double>& values, double threshold) {
    LOG_FUNCTION_ENTRY(findOutliers, values.size(), threshold);
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
    LOG_FUNCTION_EXIT(findOutliers);
}

int main() {
    std::vector<double> values = {10, 12, 12, 13, 12, 12, 14, 15, 16, 100}; // Sample data
    double zScoreThreshold = 2.0; // Define threshold for Z-score

    findOutliers(values, zScoreThreshold);

    return 0;
}