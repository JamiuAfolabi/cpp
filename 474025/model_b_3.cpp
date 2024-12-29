#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

#define STAGE1_START "Starting Stage 1: Generating sales data..."
#define STAGE1_COMPLETE "Stage 1 completed: Generated %zu sales entries."
#define STAGE2_START "Starting Stage 2: Calculating total sales..."
#define STAGE2_COMPLETE "Stage 2 completed: Calculated total sales for %zu days."
#define STAGE3_START "Starting Stage 3: Saving total sales to CSV file..."
#define STAGE3_COMPLETE "Successfully saved the total sales to %s"

struct Sale {
    std::string date; 
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

    return totalSales;
}

void saveToCSV(const std::map<std::string, double>& totalSales, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }
    
    file << "Date,TotalSales\n";
    for (const auto& entry : totalSales) {
        file << entry.first << "," << entry.second << "\n";
    }
    
    file.close();
    printf(STAGE3_COMPLETE, filename.c_str());
}

int main() {
    printf(STAGE1_START);
    std::vector<Sale> salesData = generateSalesData();
    printf(STAGE1_COMPLETE, salesData.size());
    
    printf(STAGE2_START);
    std::map<std::string, double> totalSales = calculateTotalSales(salesData);
    printf(STAGE2_COMPLETE, totalSales.size());

    const std::string filename = "total_sales.csv";
    printf(STAGE3_START);
    saveToCSV(totalSales, filename);    
    
    printf("Total Sales per Day:\n");
    for (const auto& entry : totalSales) {
        printf("%s: $%.2f\n", entry.first.c_str(), entry.second);
    }

    return 0;
}