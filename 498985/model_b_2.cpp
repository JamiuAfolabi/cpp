#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <regex>
#include <chrono>

class Event {
public:
    Event(const std::string &name, const std::string &date, double price)
        : name(name), date(date), price(price) {}

    const std::string &getName() const { return name; }
    const std::string &getDate() const { return date; }
    double getPrice() const { return price; }

private:
    std::string name;
    std::string date;
    double price;
};

class EventManager {
public:
    void registerEvent(const std::string &name, const std::string &date, double price) {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.emplace_back(name, date, price);
        std::cout << "Registered event: " << name << " on date: " << date << ", Ticket price: " << price << std::endl;
    }

    void listEvents() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto &event : events_) {
            std::cout << "Event: " << event.getName() << ", Date: " << event.getDate()
                      << ", Ticket price: " << event.getPrice() << std::endl;
        }
    }

private:
    std::vector<Event> events_;
    std::mutex mutex_;
};

// Function to parse the price from the user input string
double parsePrice(const std::string &input) {
    std::regex regex("Amount paid is (\\d+(?:\\.\\d+)?)");
    std::smatch match;

    if (std::regex_search(input, match, regex)) {
        try {
            return std::stod(match.str(1));
        } catch (const std::exception& e) {
            std::cerr << "Invalid price format. Price should be a number. Error: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Price not found in the input string." << std::endl;
    }

    return 0.0;
}

// Function for thread
void registerEventsWithPrice(EventManager &manager, const std::vector<std::pair<std::string, std::string>> &eventList) {
    for (const auto &event : eventList) {
        std::string priceInput;
        std::cout << "Enter ticket price for event '" << event.first << "': ";
        std::getline(std::cin, priceInput);
        double price = parsePrice(priceInput);
        manager.registerEvent(event.first, event.second, price);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    EventManager manager;

    std::vector<std::pair<std::string, std::string>> eventsA = {
        {"Concert", "2023-12-01"},
        {"Art Exhibition", "2023-11-15"},
    };

    std::vector<std::pair<std::string, std::string>> eventsB = {
        {"Tech Conference", "2023-10-22"},
        {"Food Festival", "2023-12-10"},
    };

    std::thread threadA(registerEventsWithPrice, std::ref(manager), eventsA);
    std::thread threadB(registerEventsWithPrice, std::ref(manager), eventsB);

    threadA.join();
    threadB.join();

    std::cout << "\nAll Registered Events:" << std::endl;
    manager.listEvents();

    return 0;
}