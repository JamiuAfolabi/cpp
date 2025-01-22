#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

// Thread-safe Logger class
class Logger {
public:
    static void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream log_file("string_handler.log", std::ios::app);
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        log_file << std::ctime(&now_time) << ": " << message << std::endl;
    }

private:
    static std::mutex mutex_;
};

std::mutex Logger::mutex_;

class StringHandler {
private:
    std::unique_ptr<char[]> message_;
    std::string timestamp_;
    std::string senderID_;
    int priority_;
    mutable std::mutex mutex_;  // For thread safety

    // Helper function to create a deep copy
    std::unique_ptr<char[]> createCopy(const char* str) {
        if (!str) return nullptr;
        size_t len = strlen(str) + 1;
        auto copy = std::make_unique<char[]>(len);
        strcpy(copy.get(), str);
        Logger::log("Allocated memory for message: " + std::string(str));
        return copy;
    }

    void generateTimestamp() {
        time_t now = time(0);
        timestamp_ = ctime(&now);
        timestamp_.erase(std::remove(timestamp_.begin(), timestamp_.end(), '\n'), 
                        timestamp_.end());
    }

public:
    // Default constructor
    StringHandler() : message_(nullptr), priority_(0) {
        generateTimestamp();
        Logger::log("Created empty StringHandler");
    }

    // Parameterized constructor
    StringHandler(const char* msg, const std::string& sender, int prio)
        : message_(createCopy(msg)), senderID_(sender), priority_(prio) {
        generateTimestamp();
        Logger::log("Created StringHandler with message from sender: " + sender);
    }

    // Copy constructor
    StringHandler(const StringHandler& other) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        message_ = createCopy(other.message_.get());
        timestamp_ = other.timestamp_;
        senderID_ = other.senderID_;
        priority_ = other.priority_;
        Logger::log("Copied StringHandler from sender: " + senderID_);
    }

    // Move constructor
    StringHandler(StringHandler&& other) noexcept {
        std::lock_guard<std::mutex> lock(other.mutex_);
        message_ = std::move(other.message_);
        timestamp_ = std::move(other.timestamp_);
        senderID_ = std::move(other.senderID_);
        priority_ = other.priority_;
        Logger::log("Moved StringHandler from sender: " + senderID_);
    }

    // Assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this != &other) {
            std::unique_lock<std::mutex> lock1(mutex_, std::defer_lock);
            std::unique_lock<std::mutex> lock2(other.mutex_, std::defer_lock);
            std::lock(lock1, lock2);  // Prevent deadlock

            message_ = createCopy(other.message_.get());
            timestamp_ = other.timestamp_;
            senderID_ = other.senderID_;
            priority_ = other.priority_;
            Logger::log("Assigned StringHandler from sender: " + senderID_);
        }
        return *this;
    }

    // Move assignment operator
    StringHandler& operator=(StringHandler&& other) noexcept {
        if (this != &other) {
            std::unique_lock<std::mutex> lock1(mutex_, std::defer_lock);
            std::unique_lock<std::mutex> lock2(other.mutex_, std::defer_lock);
            std::lock(lock1, lock2);

            message_ = std::move(other.message_);
            timestamp_ = std::move(other.timestamp_);
            senderID_ = std::move(other.senderID_);
            priority_ = other.priority_;
            Logger::log("Move assigned StringHandler from sender: " + senderID_);
        }
        return *this;
    }

    void copyMessage(const char* msg) {
        if (!msg) {
            Logger::log("Error: Attempted to copy null message");
            throw std::invalid_argument("Null pointer provided for message");
        }

        std::lock_guard<std::mutex> lock(mutex_);
        message_ = createCopy(msg);
        generateTimestamp();
        Logger::log("Copied new message from sender: " + senderID_);
    }

    size_t length() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return message_ ? strlen(message_.get()) : 0;
    }

    void concat(const char* msg) {
        if (!msg) {
            Logger::log("Error: Attempted to concatenate null message");
            throw std::invalid_argument("Null pointer provided for concatenation");
        }

        std::lock_guard<std::mutex> lock(mutex_);
        if (!message_) {
            message_ = createCopy(msg);
        } else {
            size_t current_len = strlen(message_.get());
            size_t add_len = strlen(msg);
            auto new_message = std::make_unique<char[]>(current_len + add_len + 1);
            
            strcpy(new_message.get(), message_.get());
            strcat(new_message.get(), msg);
            
            message_ = std::move(new_message);
        }
        generateTimestamp();
        Logger::log("Concatenated message for sender: " + senderID_);
    }

    // Thread-safe getters
    std::string getMessage() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return message_ ? std::string(message_.get()) : "";
    }

    std::string getTimestamp() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return timestamp_;
    }

    std::string getSenderID() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return senderID_;
    }

    int getPriority() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return priority_;
    }

    void setPriority(int prio) {
        std::lock_guard<std::mutex> lock(mutex_);
        priority_ = prio;
        Logger::log("Updated priority for sender: " + senderID_);
    }

    bool filterMessage(const std::string& profanity) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (message_ && strstr(message_.get(), profanity.c_str())) {
            Logger::log("Filtered message containing profanity from sender: " + senderID_);
            return false;
        }
        return true;
    }

    ~StringHandler() {
        Logger::log("Destroyed StringHandler from sender: " + senderID_);
        Logger::log("Deallocated memory for message from sender: " + senderID_);
    }
};

// MessageHistory class remains mostly the same but needs thread safety
class MessageHistory {
private:
    std::vector<StringHandler> messages;
    mutable std::mutex mutex_;

public:
    void addMessage(const StringHandler& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        messages.push_back(msg);
        Logger::log("Added message to history");
    }

    void batchProcessMessages(const std::vector<StringHandler>& newMsgs) {
        std::lock_guard<std::mutex> lock(mutex_);
        messages.insert(messages.end(), newMsgs.begin(), newMsgs.end());
        Logger::log("Batch processed " + std::to_string(newMsgs.size()) + " messages");
    }

    std::vector<StringHandler> searchMessages(const std::string& keyword) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<StringHandler> foundMessages;
        for (const auto& msg : messages) {
            if (msg.getMessage().find(keyword) != std::string::npos) {
                foundMessages.push_back(msg);
            }
        }
        Logger::log("Searched for messages with keyword: " + keyword);
        return foundMessages;
    }

    std::map<std::string, std::vector<StringHandler>> categorizeMessages() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::map<std::string, std::vector<StringHandler>> categories;

        for (const auto& msg : messages) {
            std::string content = msg.getMessage();
            if (content.find("urgent") != std::string::npos) {
                categories["urgent"].push_back(msg);
            } else if (content.find("info") != std::string::npos) {
                categories["info"].push_back(msg);
            } else {
                categories["general"].push_back(msg);
            }
        }

        Logger::log("Categorized messages into " + 
                   std::to_string(categories.size()) + " categories");
        return categories;
    }

    void displayMessages() const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& msg : messages) {
            std::cout << "Sender: " << msg.getSenderID()
                     << ", Timestamp: " << msg.getTimestamp()
                     << ", Priority: " << msg.getPriority()
                     << ", Message: " << msg.getMessage() << std::endl;
        }
        Logger::log("Displayed all messages");
    }
};

int main() {
    try {
        MessageHistory history;

        StringHandler msg1("Hello, this is an urgent message!", "Sender1", 1);
        StringHandler msg2("Just some information for you", "Sender2", 2);
        StringHandler msg3("This is a general message", "Sender3", 3);
        
        history.addMessage(msg1);
        history.addMessage(msg2);
        history.addMessage(msg3);

        std::cout << "All Messages:" << std::endl;
        history.displayMessages();

        auto foundMessages = history.searchMessages("urgent");
        std::cout << "\nMessages containing 'urgent':" << std::endl;
        for (const auto& msg : foundMessages) {
            std::cout << msg.getMessage() << std::endl;
        }

        auto categorizedMessages = history.categorizeMessages();
        std::cout << "\nCategorized Messages:" << std::endl;
        for (const auto& category : categorizedMessages) {
            std::cout << "[" << category.first << "]:" << std::endl;
            for (const auto& msg : category.second) {
                std::cout << " - " << msg.getMessage() << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        Logger::log("Error in main: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
