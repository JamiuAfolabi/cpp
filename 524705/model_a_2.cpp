#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>

class StringHandler {
private:
    char* message;
    std::string timestamp;
    std::string senderID;
    int priority;

public:
    // Default constructor
    StringHandler() : message(nullptr), priority(0) {
        generateTimestamp();
    }

    // Parameterized constructor
    StringHandler(const char* msg, const std::string& sender, int prio)
        : priority(prio), senderID(sender) {
        generateTimestamp();
        if (msg) {
            message = new char[strlen(msg) + 1];
            strcpy(message, msg);
        } else {
            message = nullptr;
        }
    }

    // Copy constructor
    StringHandler(const StringHandler& other) {
        if (other.message) {
            message = new char[strlen(other.message) + 1];
            strcpy(message, other.message);
        } else {
            message = nullptr;
        }
        timestamp = other.timestamp;
        senderID = other.senderID;
        priority = other.priority;
    }

    // Assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this != &other) {
            // Clean up existing message
            delete[] message;

            if (other.message) {
                message = new char[strlen(other.message) + 1];
                strcpy(message, other.message);
            } else {
                message = nullptr;
            }

            timestamp = other.timestamp;
            senderID = other.senderID;
            priority = other.priority;
        }
        return *this;
    }

    // Destructor
    ~StringHandler() {
        delete[] message;
    }

    // Function for safe string copying
    void copyMessage(const char* msg) {
        delete[] message; // Clean up existing message
        if (msg) {
            message = new char[strlen(msg) + 1];
            strcpy(message, msg);
            generateTimestamp(); // Update timestamp on message change
        } else {
            throw std::invalid_argument("Null pointer provided for message");
        }
    }

    // Function for calculating length of the message
    size_t length() const {
        return message ? strlen(message) : 0;
    }

    // Function for concatenating messages
    void concat(const char* msg) {
        if (msg) {
            size_t newLength = length() + strlen(msg);
            char* newMessage = new char[newLength + 1];

            if (message) {
                strcpy(newMessage, message); // Copy existing message
            }
            strcat(newMessage, msg); // Concatenate new message

            delete[] message; // Clean up old message
            message = newMessage; // Assign new message
            generateTimestamp(); // Update timestamp on message change
        } else {
            throw std::invalid_argument("Null pointer provided for message");
        }
    }

    // Generate a timestamp
    void generateTimestamp() {
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.erase(std::remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
    }

    // Get metadata
    std::string getTimestamp() const { return timestamp; }
    std::string getSenderID() const { return senderID; }
    int getPriority() const { return priority; }

    // Set priority
    void setPriority(int prio) { priority = prio; }

    // Get the message (for demonstration)
    const char* getMessage() const {
        return message ? message : "";
    }

    // Filtering function (example: filter out messages that contain profanity)
    bool filterMessage(const std::string& profanity) const {
        if (message && strstr(message, profanity.c_str())) {
            return false;
        }
        return true;
    }
};

// Class to manage message history
class MessageHistory {
private:
    std::vector<StringHandler> messages;

public:
    // Add a message to history
    void addMessage(const StringHandler& msg) {
        messages.push_back(msg);
    }

    // Batch process messages
    void batchProcessMessages(const std::vector<StringHandler>& newMsgs) {
        for (const auto& msg : newMsgs) {
            addMessage(msg);
        }
    }

    // Search messages
    std::vector<StringHandler> searchMessages(const std::string& keyword) const {
        std::vector<StringHandler> foundMessages;
        for (const auto& msg : messages) {
            if (strstr(msg.getMessage(), keyword.c_str())) {
                foundMessages.push_back(msg);
            }
        }
        return foundMessages;
    }

    // Message categorization based on content (simple categorization)
    std::map<std::string, std::vector<StringHandler>> categorizeMessages() const {
        std::map<std::string, std::vector<StringHandler>> categories;

        for (const auto& msg : messages) {
            std::string content(msg.getMessage());
            if (content.find("urgent") != std::string::npos) {
                categories["urgent"].push_back(msg);
            } else if (content.find("info") != std::string::npos) {
                categories["info"].push_back(msg);
            } else {
                categories["general"].push_back(msg);
            }
        }

        return categories;
    }

    // Display all messages
    void displayMessages() const {
        for (const auto& msg : messages) {
            std::cout << "Sender: " << msg.getSenderID()
                      << ", Timestamp: " << msg.getTimestamp()
                      << ", Priority: " << msg.getPriority()
                      << ", Message: " << msg.getMessage() << std::endl;
        }
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

        // Search for messages containing "urgent"
        auto foundMessages = history.searchMessages("urgent");
        std::cout << "\nMessages containing 'urgent':" << std::endl;
        for (const auto& msg : foundMessages) {
            std::cout << msg.getMessage() << std::endl;
        }

        // Categorize messages
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
    }

    return 0;
}