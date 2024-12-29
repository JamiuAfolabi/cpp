#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

struct Sale {
    std::string date; // Format: YYYY-MM-DD
    double amount;
};

std::vector<Sale> generateSalesData() {
    return {
        {"2023-10-01", 150.25},
        {"2023-10-01", 75.50},
        {"2023-10-02", 100.00},
        {"2023-10-02", 200.75},
        {"2023-10-03", 50.00}
    };
}

std::map<std::string, double> calculateTotalSales(const std::vector<Sale>& sales) {
    std::map<std::string, double> totalSales;
    
    for (const auto& sale : sales) {
        totalSales[sale.date] += sale.amount;
    }
    std::cout << "Total sales per day calculation completed.\n";
    return totalSales;
}

void saveDataToCSV(const std::map<std::string, double>& totalSales, const std::string& filePath) {
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cout << "Error opening output file!".endl;
        return;
    }

    outputFile << "Date,Total_Sales\n";

    for (const auto& entry : totalSales) {
        outputFile << entry.first << "," << entry.second << "\n";
    }

    outputFile.close();
    std::cout << "Data successfully saved to '" << filePath << "'".endl;
}

int main() {
    std::vector<Sale> salesData = generateSalesData();
    std::cout << "Sales data generation completed.\n";
    
    std::map<std::string, double> totalSales = calculateTotalSales(salesData);
    
    // Output total sales per day
    std::cout << "\nTotal Sales per Day:\n";
    for (const auto& entry : totalSales) {
        std::cout << entry.first << ": $" << entry.second << std::endl;
    }
    
    std::string outputFilePath = "total_sales.csv";  // Change the filename as per your requirement
    saveDataToCSV(totalSales, outputFilePath);

    return 0;
}