#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

struct Sale {
    std::string date; // Format: YYYY-MM-DD
    double amount;
};

// Function to generate sales data
std::vector<Sale> generateSalesData() {
    // Example sales data
    return {
        {"2023-10-01", 150.25},
        {"2023-10-01", 75.50},
        {"2023-10-02", 100.00},
        {"2023-10-02", 200.75},
        {"2023-10-03", 50.00}
    };
}

// Function to process sales data and calculate total sales per day
std::map<std::string, double> calculateTotalSales(const std::vector<Sale>& sales) {
    std::map<std::string, double> totalSales;

    for (const auto& sale : sales) {
        totalSales[sale.date] += sale.amount;
    }

    return totalSales;
}

// Function to save total sales to a CSV file
void saveToCSV(const std::map<std::string, double>& totalSales, const std::string& filename) {
    std::ofstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Write CSV header
    file << "Date,TotalSales\n";

    // Write total sales data
    for (const auto& entry : totalSales) {
        file << entry.first << "," << entry.second << "\n";
    }

    file.close();
    printf("Successfully saved the total sales to %s\n", filename.c_str());
}

// Main function
int main() {
    // Stage 1: Generate sales data
    printf("Starting Stage 1: Generating sales data...\n");
    std::vector<Sale> salesData = generateSalesData();
    printf("Stage 1 completed: Generated %zu sales entries.\n", salesData.size());

    // Stage 2: Calculate total sales for each day
    printf("Starting Stage 2: Calculating total sales...\n");
    std::map<std::string, double> totalSales = calculateTotalSales(salesData);
    printf("Stage 2 completed: Calculated total sales for %zu days.\n", totalSales.size());

    // Stage 3: Save total sales to CSV
    const std::string filename = "total_sales.csv";
    printf("Starting Stage 3: Saving total sales to CSV file...\n");
    saveToCSV(totalSales, filename);

    // Output total sales per day
    printf("Total Sales per Day:\n");
    for (const auto& entry : totalSales) {
        printf("%s: $%.2f\n", entry.first.c_str(), entry.second);
    }

    return 0;
}