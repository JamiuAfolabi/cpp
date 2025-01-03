#include <iostream>
#include <fstream>
#include <string>

class Command {
public:
    virtual void execute(const std::string& data) = 0;
};

class WriteToFileCommand : public Command {
public:
    void execute(const std::string& data) override {
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

class Query {
public:
    virtual void execute() = 0;
};

class ReadFromFileQuery : public Query {
public:
    void execute() override {
        std::ifstream inFile("output.txt");
        std::string line;

        if (inFile.is_open()) {
            std::cout << "Contents of the file:" << std::endl;
            while (std::getline(inFile, line)) {
                std::cout << line << std::endl;
            }
            inFile.close();
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }
};

int main() {
    std::string userInput;
    WriteToFileCommand writeCommand;
    ReadFromFileQuery readQuery;

    // Collecting user input
    std::cout << "Enter some text to save to the file (type 'exit' to quit):" << std::endl;

    while (true) {
        std::getline(std::cin, userInput);
        if (userInput == "exit") {
            break;
        }

        // Execute the command to write input to file
        writeCommand.execute(userInput);
    }

    // Execute the query to read from the file
    readQuery.execute();

    return 0;
}