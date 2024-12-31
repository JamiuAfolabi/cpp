#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <regex>
#include <optional>

class Event {
public:
    Event(const std::string &name, const std::string &date, double price)
        : name(name), date(date), ticketPrice(price) {}
    // Rest of the class remains the same
    const std::string &getName() const { return name; }
    const std::string &getDate() const { return date; }
    double getTicketPrice() const { return ticketPrice; }

private:
    std::string name;
    std::string date;
    double ticketPrice;
};

class EventManager {
public:
    // Rest of the class remains the same
    void registerEvent(const std::string &name, const std::string &date, double price) {
        std::lock_guard<std::mutex> lock(mutex_); // Lock for thread safety
        events_.emplace_back(name, date, price);
        std::cout << "Registered event: " << name << " on date: " << date << " with price: " << price << " naira\n";
    }

    void listEvents() {
        std::lock_guard<std::mutex> lock(mutex_); // Lock for thread safety
        for (const auto &event : events_) {
            std::cout << "Event: " << event.getName()
                      << ", Date: " << event.getDate()
                      << ", Ticket Price: " << event.getTicketPrice() << " naira\n";
        }
    }

private:
    std::vector<Event> events_;
    std::mutex mutex_; // Mutex for thread safety
};

// Function prototype for extractPriceFromDescription
std::optional<double> extractPriceFromDescription(const std::string &description);

void registerEvents(EventManager &manager, const std::vector<std::tuple<std::string, std::string, std::string>> &eventList) {
    for (const auto &event : eventList) {
        std::string name = std::get<0>(event);
        std::string date = std::get<1>(event);
        std::string description = std::get<2>(event);

        if (name.empty()) {
            std::cerr << "Error: Event name is required." << std::endl;
            continue;
        }
        
        if (date.empty()) {
            std::cerr << "Error: Event date is required." << std::endl;
            continue;
        }
        
        std::optional<double> price = extractPriceFromDescription(description);
        
        if (!price) {
            std::cerr << "Error: Failed to extract price from description: " << description << std::endl;
            continue;
        }

        manager.registerEvent(name, date, *price);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
    }
}

std::optional<double> extractPriceFromDescription(const std::string &description) {
    std::regex priceRegex(R"(\b(\d+)\s+naira\b)");
    std::smatch match;

    if (std::regex_search(description, match, priceRegex) && match.size() > 1) {
        try {
            return std::stod(match[1].str()); // Convert matched string to double
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid price format: " << match[1].str() << std::endl;
        }
    }
    return std::nullopt; // No valid price found
}

int main() {
    EventManager manager;

    // Example event lists
    std::vector<std::tuple<std::string, std::string, std::string>> eventsA = {
        {"Concert", "2023-12-01", "Amount paid is 5000 naira"},
        {"Art Exhibition", "", "Tickets cost 2500 naira"}, // Empty date
        {"Tech Conference", "2023-10-22", "Invalid price description"},
        {"Food Festival", "2023-12-10", "Entrance fee is three thousand naira"}, // Invalid price format
    };
    std::vector<std::tuple<std::string, std::string, std::string>> eventsB = {
        {"Tech Conference", "2023-10-22", "Overall cost is 8000 naira"},
        {"Food Festival", "2023-12-10", "Entrance fee is 3500 naira"},
    };

    // Create two threads to register events concurrently
    std::thread threadA(registerEvents, std::ref(manager), eventsA);
    std::thread threadB(registerEvents, std::ref(manager), eventsB);

    // Wait for threads to finish
    threadA.join();
    threadB.join();

    // List all registered events
    std::cout << "\nAll Registered Events:\n";
    manager.listEvents();

    // Rest of the code remains the same
    return 0;
}
