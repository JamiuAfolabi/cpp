
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Command Class: Responsible for writing data to the file
class WriteDataCommand {
private:
    string fileName;

public:
    WriteDataCommand(const string& fileName) : fileName(fileName) {}

    void execute(const string& data) {
        ofstream file(fileName);
        if (file.is_open()) {
            file << data << endl;
            file.close();
            cout << "Data written to file successfully." << endl;
        } else {
            cout << "Error: Failed to open the file." << endl;
        }
    }
};

// Query Class: Responsible for taking user input
class UserInputQuery {
public:
    string execute() {
        string data;
        cout << "Enter data to write to the file: ";
        getline(cin, data);
        return data;
    }
};

int main() {
    string fileName;
    cout << "Enter the name of the text file: ";
    getline(cin, fileName);

    // Create instances of Query and Command classes
    UserInputQuery inputQuery;
    WriteDataCommand writeCommand(fileName);

    // Query the user for input
    string dataToWrite = inputQuery.execute();

    // Execute the command to write the data to the file
    writeCommand.execute(dataToWrite);

    return 0;
}

