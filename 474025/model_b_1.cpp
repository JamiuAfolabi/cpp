#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>  // For using setw
#include <ctime>
#include <cstdlib>

using namespace std;

class SalesData {
public:
    string date;
    double sales;

    SalesData(string date, double sales): date(date), sales(sales) {}
};

int main() {
    const int num_entries = 5;
    // Generate random sales data entries
    unordered_map<string, double> daily_sales;
    SalesData salesData[num_entries];

    srand(static_cast<unsigned>(time(0))); // seed random number generator

    for (int i = 0; i < num_entries; i++) {
       // Generate a random date between 2023-01-01 and 2023-12-31
        string day = to_string(1 + rand() % 31);
        string month = to_string(1 + rand() % 12);
        if (month.size() == 1) {
            month = "0" + month;
        }
        if (day.size() == 1) {
            day = "0" + day;
        }
        string date = "2023-" + month + "-" + day;
        double sales = static_cast<double>(rand()) / RAND_MAX * 1000;
        salesData[i] = SalesData(date, sales);
        daily_sales[date] += sales;
    }
    cout << "\n-- Sales Data Entries --\n";
    for (int i = 0; i < num_entries; i++) {
        cout << "Date: " << salesData[i].date << " | Sales: $" << fixed << setprecision(2) << salesData[i].sales << endl;
    }
 
    cout << "\n-- Total Sales by Date --\n";
    cout << setw(10) << "Date" << setw(10) << "Total Sales" << endl;
    for (const auto& entry : daily_sales) {
        cout << setw(10) << entry.first << setw(10) << fixed << setprecision(2) << entry.second << endl;
    }
    cout << "\n";

    return 0;
}
