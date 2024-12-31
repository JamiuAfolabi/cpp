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

// Custom exception for price extraction errors
class PriceExtractionException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Price could not be extracted from description.";
    }
};

// Function to extract price from description
std::optional<double> extractPriceFromDescription(const std::string &description) {
    std::regex priceRegex(R"(\b(\d+)\s+naira\b)");
    std::smatch match;

    if (std::regex_search(description, match, priceRegex) && match.size() > 1) {
        return std::stod(match[1].str()); // Convert matched string to double
    }
    return std::optional<double>(); // Return empty optional if no price found
}

// Function for thread
void registerEvents(EventManager &manager, const std::vector<std::tuple<std::string, std::string, std::string>> &eventList) {
    for (const auto &event : eventList) {
        std::string name = std::get<0>(event);
        std::string date = std::get<1>(event);
        std::string description = std::get<2>(event);
        
        try {
            auto priceOpt = extractPriceFromDescription(description);
            if (!priceOpt) {
                throw PriceExtractionException();
            }
            manager.registerEvent(name, date, *priceOpt);
        } catch (const std::exception &e) {
            std::cerr << "Error registering event \"" << name << "\": " << e.what() << "\n";
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
    }
}

int main() {
    EventManager manager;

    // Example event lists with possible invalid descriptions
    std::vector<std::tuple<std::string, std::string, std::string>> eventsA = {
        {"Concert", "2023-12-01", "Amount paid is 5000 naira"},
        {"Art Exhibition", "2023-11-15", "Tickets cost 2500 naira"},
        {"Invalid Event", "2023-12-02", "No price noted here"},
    };

    std::vector<std::tuple<std::string, std::string, std::string>> eventsB = {
        {"Tech Conference", "2023-10-22", "Overall cost is 8000 naira"},
        {"Food Festival", "2023-12-10", "Entrance fee is free"},
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

    return 0;
} 