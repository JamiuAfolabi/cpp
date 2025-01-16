#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

class StreamHandler {
public:
    StreamHandler(const std::string& filename);
    ~StreamHandler();

    bool readNextTransaction(std::vector<std::string>& transaction);
    void rollback();
    void seek(long position);
    long tell() const;

private:
    std::ifstream fileStream;
    std::string buffer;
    size_t bufferIndex;
    std::stack<std::streampos> positions;
    std::streampos lastPosition;

    bool readNextLine();
    bool parseLine(const std::string& line, std::vector<std::string>& transaction);
    void handleMalformedData(const std::string& line);
};

StreamHandler::StreamHandler(const std::string& filename) 
    : bufferIndex(0), lastPosition(0) {
    fileStream.open(filename);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
}

StreamHandler::~StreamHandler() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

bool StreamHandler::readNextTransaction(std::vector<std::string>& transaction) {
    std::string line;
    if (!readNextLine()) {
        return false; // End of file or error
    }

    if (parseLine(line, transaction)) {
        positions.push(fileStream.tellg());
        return true;
    } else {
        handleMalformedData(line);
        return false; // Malformed data situation
    }
}

bool StreamHandler::readNextLine() {
    std::getline(fileStream, buffer);
    if (fileStream.eof()) {
        return false; // No more lines to read
    } else if (fileStream.fail()) {
        throw std::runtime_error("Error reading from file.");
    }
    lastPosition = fileStream.tellg();
    return true;
}

bool StreamHandler::parseLine(const std::string& line, std::vector<std::string>& transaction) {
    transaction.clear();
    std::istringstream ss(line);
    std::string field;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes; // Toggle quote
        } else if (ch == ',' && !inQuotes) {
            transaction.push_back(field);
            field.clear();
            continue;
        }
        field += ch;
    }
    transaction.push_back(field); // Push last field

    return true; // Assuming parsing was successful for this simple example
}

void StreamHandler::rollback() {
    if (!positions.empty()) {
        fileStream.seekg(positions.top());
        positions.pop();
    }
}

void StreamHandler::seek(long position) {
    fileStream.seekg(position);
    if (fileStream.fail()) {
        throw std::runtime_error("Failed to seek to position.");
    }
}

long StreamHandler::tell() const {
    return static_cast<long>(fileStream.tellg());
}

void StreamHandler::handleMalformedData(const std::string& line) {
    std::cerr << "Malformed data encountered: " << line << std::endl;
    // Implement further error recovery logic if necessary
}

// Example usage
int main() {
    try {
        StreamHandler handler("transactions.csv");
        std::vector<std::string> transaction;

        while (handler.readNextTransaction(transaction)) {
            for (const auto& field : transaction) {
                std::cout << field << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}