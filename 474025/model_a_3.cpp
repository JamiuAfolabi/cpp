#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

// Define sale struct
struct Sale {
    std::string date; // Format: YYYY-MM-DD
    double amount;
};

// Preprocessor directives for constants
#define STAGE_START_1 "Starting Stage 1: Generating sales data...\n"
#define STAGE_COMPLETE_1 "Stage 1 completed: Generated %zu sales entries.\n"
#define STAGE_START_2 "Starting Stage 2: Calculating total sales...\n"
#define STAGE_COMPLETE_2 "Stage 2 completed: Calculated total sales for %zu days.\n"
#define STAGE_START_3 "Starting Stage 3: Saving total sales to CSV file...\n"
#define STAGE_COMPLETE_3_FMT "Successfully saved the total sales to %s\n"
#define TOTAL_SALES_HEADER "Date,TotalSales\n"

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
    file << TOTAL_SALES_HEADER;

    // Write total sales data
    for (const auto& entry : totalSales) {
        file << entry.first << "," << entry.second << "\n";
    }

    file.close();
    printf(STAGE_COMPLETE_3_FMT, filename.c_str());
}

// Main function
int main() {
    // Stage 1: Generate sales data
    printf(STAGE_START_1);
    std::vector<Sale> salesData = generateSalesData();
    printf(STAGE_COMPLETE_1, salesData.size());

    // Stage 2: Calculate total sales for each day
    printf(STAGE_START_2);
    std::map<std::string, double> totalSales = calculateTotalSales(salesData);
    printf(STAGE_COMPLETE_2, totalSales.size());

    // Stage 3: Save total sales to CSV
    const std::string filename = "total_sales.csv";
    printf(STAGE_START_3);
    saveToCSV(totalSales, filename);

    // Output total sales per day
    printf("Total Sales per Day:\n");
    for (const auto& entry : totalSales) {
        printf("%s: $%.2f\n", entry.first.c_str(), entry.second);
    }

    return 0;
}