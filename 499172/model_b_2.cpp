#include <iostream>
#include <fstream>
#include <string>
#include <cstring>   // For strlen function

class WriteToFileCommand : public Command {
public:
    void execute(const std::string& data) override {
        if (strlen(data.c_str()) > 50) {
            std::cout << "Input length should not exceed 50 characters. Data not written." << std::endl;
            return;
        }

        std::ofstream outFile("output.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << data << std::endl;
            outFile.close();
            std::cout << "Data written to file successfully." << std::endl;
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }
};