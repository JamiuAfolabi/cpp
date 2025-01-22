#include <iostream>
#include <cstring>
#include <stdexcept>

class StringHandler {
private:
    char* message;
    std::string timestamp;
    std::string senderID;
    int priority;

public:
    struct MessageMetadata {
        std::string timestamp;
        std::string senderID;
        int priority;
    };

    // Default constructor
    StringHandler() : message(nullptr) {}

    // Parameterized constructor
    StringHandler(const char* msg) {
        if (msg) {
            // Allocate memory and copy the message
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
    }

    // Assignment operator
    StringHandler& operator=(const StringHandler& other) {
        if (this != &other) {
            // Clean up existing message
            delete[] message;

            if (other.message) {
                // Allocate new memory and copy the message
                message = new char[strlen(other.message) + 1];
                strcpy(message, other.message);
            } else {
                message = nullptr;
            }
        }
        return *this;
    }

    // Destructor
    ~StringHandler() {
        delete[] message;
    }

    // Function for safe string copying
    void copyMessage(const char* msg) {
        if (msg) {
            delete[] message; // Clean up existing message
            message = new char[strlen(msg) + 1];
            strcpy(message, msg);
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
        } else {
            throw std::invalid_argument("Null pointer provided for message");
        }
    }

    // Function to get the message (for demonstration)
    const char* getMessage() const {
        return message ? message : "";
    }

    void setTimestamp(const std::string& ts) { timestamp = ts; }
    void setSenderID(const std::string& sid) { senderID = sid; }
    void setPriority(int prio) { priority = prio; }
    const std::string& getTimestamp() const { return timestamp; }
    const std::string& getSenderID() const { return senderID; }
    int getPriority() const { return priority; }

    void storeMessage(const char* msg, const std::string& ts = "", const std::string& sid = "", int prio = 0) {
        copyMessage(msg);
        setTimestamp(ts);
        setSenderID(sid);
        setPriority(prio);
    }

    // Message history tracking system using a vector
    std::vector<StringHandler> messageHistory;

    void appendToHistory() {
        messageHistory.push_back(*this);
    }

    void displayHistory() const {
        for (size_t i = 0; i < messageHistory.size(); ++i) {
            const StringHandler& msg = messageHistory[i];
            std::cout << "Message " << i + 1 << ":\n";
            std::cout << "  Message: " << msg.getMessage() << "\n";
            std::cout << "  Timestamp: " << msg.getTimestamp() << "\n";
            std::cout << "  Sender ID: " << msg.getSenderID() << "\n";
            std::cout << "  Priority: " << msg.getPriority() << "\n";
            std::cout << "-------------\n";
        }
    }

    // Search functionality within stored messages
    bool searchMessage(const char* substring) const {
        for (const StringHandler& msg : messageHistory) {
            if (strstr(msg.getMessage(), substring)) {
                return true;
            }
        }
        return false;
    }

    // Message categorization based on content (simple keyword-based)
    enum class MessageCategory {
        GENERAL,
        URGENT,
        INFORMATION
    };

    MessageCategory categorizeMessage() const {
        const char* msg = getMessage();
        if (strstr(msg, "urgent")) {
            return MessageCategory::URGENT;
        } else if (strstr(msg, "info")) {
            return MessageCategory::INFORMATION;
        } else {
            return MessageCategory::GENERAL;
        }
    }

private:
    static std::unordered_map<MessageCategory, std::vector<StringHandler>> categorizedMessages;
};

std::unordered_map<StringHandler::MessageCategory, std::vector<StringHandler>> StringHandler::categorizedMessages;

void StringHandler::appendToCategory() {
    const MessageCategory category = categorizeMessage();
    categorizedMessages[category].push_back(*this);
}

void StringHandler::displayCategorizedMessages(StringHandler::MessageCategory category) const {
    const std::vector<StringHandler>& msgs = categorizedMessages[category];
    if (msgs.empty()) {
        std::cout << "No messages in category: " << category << std::endl;
    } else {
        for (size_t i = 0; i < msgs.size(); ++i) {
            const StringHandler& msg = msgs[i];
            std::cout << "Message " << i + 1 << " in category " << category << ":\n";
            std::cout << "  Message: " << msg.getMessage() << "\n";
            std::cout << "  Timestamp: " << msg.getTimestamp() << "\n";
            std::cout << "  Sender ID: " << msg.getSenderID() << "\n";
            std::cout << "  Priority: " << msg.getPriority() << "\n";
            std::cout << "-------------\n";
        }
    }
}


int main() {
    try {
        StringHandler sh1("Hello");
        StringHandler sh2;
        
        // Testing copy constructor
        sh2 = sh1; // Using assignment operator
        std::cout << "Message in sh2: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

        // Testing concatenation
        sh2.concat(" World!");
        std::cout << "After concatenation: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

        // Testing safe copying
        sh2.copyMessage("New Message");
        std::cout << "After copying: " << sh2.getMessage() << " (Length: " << sh2.length() << ")\n";

        StringHandler sh1("Hello World!");
        sh1.setTimestamp(std::to_string(time(0)));
        sh1.setSenderID("sender1");
        sh1.appendToHistory();

        sh1.concat(" How are you?");
        sh1.appendToHistory();

        sh1.storeMessage("Urgent: System failure!", "2024-08-03 10:15:30", "sender2", 5);
        sh1.appendToHistory();
        sh1.appendToCategory();

        sh1.displayHistory();

        if (sh1.searchMessage("System")) {
            std::cout << "Message found containing 'System'!\n";
        } else {
            std::cout << "Message not found!\n";
        }

        sh1.displayCategorizedMessages(StringHandler::MessageCategory::URGENT);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}