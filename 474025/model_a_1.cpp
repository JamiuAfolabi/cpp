#include <iostream>
#include <map>
#include <string>
#include <vector>

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

// Main function
int main() {
    // Generate sales data
    std::vector<Sale> salesData = generateSalesData();

    // Calculate total sales for each day
    std::map<std::string, double> totalSales = calculateTotalSales(salesData);

    // Output total sales per day
    std::cout << "Total Sales per Day:\n";
    for (const auto& entry : totalSales) {
        std::cout << entry.first << ": $" << entry.second << std::endl;
    }

    return 0;
}