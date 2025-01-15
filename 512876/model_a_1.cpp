#include <iostream>
#include <vector>
#include <cmath>

double calculateMean(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    return sum / values.size();
}

double calculateStandardDeviation(const std::vector<double>& values, double mean) {
    double sum = 0.0;
    for (double value : values) {
        sum += (value - mean) * (value - mean);
    }
    return std::sqrt(sum / values.size());
}

void findOutliers(const std::vector<double>& values, double threshold) {
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
}

int main() {
    std::vector<double> values = {10, 12, 12, 13, 12, 12, 14, 15, 16, 100}; // Sample data
    double zScoreThreshold = 2.0; // Define threshold for Z-score

    findOutliers(values, zScoreThreshold);

    return 0;
}