#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <ctime>
#include <stdexcept>

class StringHandler {
private:
    std::unique_ptr<char[]> message;
    std::string timestamp;
    std::string senderID;
    int priority;

    // Logging file
    static std::ofstream logFile;

    // Mutex for thread safety
    static std::mutex mtx;

public:
    static void initializeLog() {
        std::time_t now = std::time(0);
        std::tm tm = *std::localtime(&now);
        logFile.open("StringHandler_Log_" + std::to_string(tm.tm_year + 1900) +
                     "_" + std::to_string(tm.tm_mon + 1) + "_" +
                     std::to_string(tm.tm_mday) + ".log");
    }

    // Default constructor
    StringHandler() : priority(0) {
        generateTimestamp();
        message.reset(nullptr);
        logAllocation();
    }

    // Parameterized constructor
    StringHandler(const char* msg, const std::string& sender, int prio)
        : priority(prio), senderID(sender) {
        generateTimestamp();
        if (msg) {
            size_t length = strlen(msg) + 1;
            message.reset(new char[length]);
            strcpy(message.get(), msg);
        } else {
            message.reset(nullptr);
        }
        logAllocation();
    }

    // Copy constructor
    StringHandler(const StringHandler& other)
        : senderID(other.senderID), priority(other.priority) {
        generateTimestamp();
        if (other.message) {
            size_t length = strlen(other.message.get()) + 1;
            message.reset(new char[length]);
            strcpy(message.get(), other.message.get());
        } else {
            message.reset(nullptr);
        }
        logAllocation();
    }

    // Move constructor
    StringHandler(StringHandler&& other) noexcept
        : message(std::move(other.message)),
          timestamp(std::move(other.timestamp)),
          senderID(std::move(other.senderID)),
          priority(other.priority) {
        other.message.reset(nullptr);
        logAllocation();
    }

    // Copy assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this != &other) {
            std::unique_ptr<char[]> temp(new char[strlen(other.message.get()) + 1]);
            strcpy(temp.get(), other.message.get());
            std::swap(message, temp);
            senderID = other.senderID;
            priority = other.priority;
        }
        return *this;
    }

    // Move assignment operator
    StringHandler& operator=(StringHandler&& other) noexcept {
        if (this != &other) {
            message.swap(other.message);
            senderID = std::move(other.senderID);
            priority = other.priority;
        }
        return *this;
    }

    // Destructor
    ~StringHandler() {
        logDeallocation();
    }

    // Function for safe string copying
    void copyMessage(const char* msg) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (msg) {
                size_t length = strlen(msg) + 1;
                std::unique_ptr<char[]> temp(new char[length]);
                strcpy(temp.get(), msg);
                std::swap(message, temp);
            } else {
                message.reset(nullptr);
            }
            generateTimestamp();
        }
        logAllocation();
    }

    size_t length() const {
        return message ? strlen(message.get()) : 0;
    }

    void concat(const char* msg) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (msg) {
                size_t newLength = length() + strlen(msg) + 1;
                std::unique_ptr<char[]> temp(new char[newLength]);
                if (message) {
                    strcpy(temp.get(), message.get());
                }
                strcat(temp.get(), msg);
                std::swap(message, temp);
            }
            generateTimestamp();
        }
        logAllocation();
    }

    void generateTimestamp() {
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.erase(std::remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
    }

    std::string getTimestamp() const { return timestamp; }
    std::string getSenderID() const { return senderID; }
    int getPriority() const { return priority; }

    void setPriority(int prio) { priority = prio; }

    const char* getMessage() const {
        return message ? message.get() : "";
    }

private:
    void logAllocation() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            logFile << "[" << timestamp << "] " << senderID << ": Allocated " << (message ? length() + 1 : 0) << " bytes" << std::endl;
        }
    }

    void logDeallocation() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            logFile << "[" << timestamp << "] " << senderID << ": Deallocated " << (message ? length() + 1 : 0) << " bytes" << std::endl;
        }
    }
};


int main() {
    // Initialize logging
    StringHandler::initializeLog();

    // Create StringHandler objects
    StringHandler sh1("Hello, World!", "User1", 1);
    StringHandler sh2("Welcome to C++ programming.", "User2", 2);

    // Output the message and details
    std::cout << "Sender: " << sh1.getSenderID() << ", Message: " << sh1.getMessage() << ", Timestamp: " << sh1.getTimestamp() << std::endl;
    std::cout << "Sender: " << sh2.getSenderID() << ", Message: " << sh2.getMessage() << ", Timestamp: " << sh2.getTimestamp() << std::endl;

    // Modify message using concat
    sh1.concat(" Let's learn multithreading!");

    // Output the updated message
    std::cout << "Updated Message: " << sh1.getMessage() << std::endl;

    return 0;
}
