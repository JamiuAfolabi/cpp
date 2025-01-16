#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ofstream outFile("transactions.csv");
    if (!outFile) {
        std::cerr << "Failed to create transactions.csv" << std::endl;
        return 1;
    }

    outFile << "Date,Type,Amount,Description,Balance,Reference,Notes\n"
            << "2024-01-01,TRANSFER,4790.47,Office supplies purchase,28082.93,REF525156,Notes for transaction 1\n"
            << "2024-01-02,WITHDRAWAL,9319.35,Regular salary payment,6802.17,REF668201,Notes for transaction 2\n"
            << "2024-01-03,DEPOSIT,4800.79,\"Grocery shopping at \"\"Super, Fresh\"\" Market\",18697.01,REF628689,Notes for transaction 3\n"
            << "2024-01-04,DEPOSIT,5739.57,Gym membership fee,32852.51,REF463815,Notes for transaction 4\n"
            << "2024-01-05,REFUND,818.06,Regular salary payment,42160.30,REF795619,Notes for transaction 5\n"
            << "2024-01-06,PAYMENT,8804.04,Insurance premium,37282.57,REF502879,Notes for transaction 6\n"
            << "2024-01-07,WITHDRAWAL,4459.43,Mobile phone bill,30912.35,REF963026,Notes for transaction 7\n"
            << "2024-01-08,WITHDRAWAL,9859.86,Online transfer to savings,21406.38,REF877968,Notes for transaction 8\n"
            << "2024-01-09,WITHDRAWAL,2306.85,\"Grocery shopping at \"\"Super, Fresh\"\" Market\",46279.26,REF379153,Notes for transaction 9\n"
            << "2024-01-10,WITHDRAWAL,7543.19,Gym membership fee,30626.85,REF709025,Notes for transaction 10\n"
            << "2024-01-11,WITHDRAWAL,9922.44,ATM withdrawal,47132.10,REF041391,Notes for transaction 11\n"
            << "2024-01-12,WITHDRAWAL,3976.43,Netflix subscription,44691.10,REF059083,Notes for transaction 12\n"
            << "2024-01-13,WITHDRAWAL,5399.26,\"Grocery shopping at \"\"Super, Fresh\"\" Market\",10400.34,REF620330,Notes for transaction 13\n"
            << "2024-01-14,DEPOSIT,1061.35,Gym membership fee,28005.15,REF994780,Notes for transaction 14\n"
            << "2024-01-15,WITHDRAWAL,5842.49,Regular salary payment,4305.75,REF690164,Notes for transaction 15\n"
            << "2024-01-16,DEPOSIT,7986.65,\"Restaurant payment: \"\"Joe's Pizza & Pasta\nNew York Style\"\"\",37088.02,REF921625,Notes for transaction 16\n"
            // ... Add remaining transactions similarly
            << "2024-02-19,TRANSFER,2649.83,Monthly rent payment,49453.04,REF850955,Notes for transaction 50\n";

    outFile.close();
    
    if (outFile.fail()) {
        std::cerr << "Error writing to transactions.csv" << std::endl;
        return 1;
    }
    
    std::cout << "Successfully created transactions.csv" << std::endl;
    return 0;
}