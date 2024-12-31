#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

class Event {
public:
    Event(const std::string &name, const std::string &date)
        : name(name), date(date) {}

    const std::string &getName() const { return name; }
    const std::string &getDate() const { return date; }

private:
    std::string name;
    std::string date;
};

class EventManager {
public:
    // Register an event thread-safely
    void registerEvent(const std::string &name, const std::string &date) {
        std::lock_guard<std::mutex> lock(mutex_); // Lock for thread safety
        events_.emplace_back(name, date);
        std::cout << "Registered event: " << name << " on date: " << date << std::endl;
    }

    // List events
    void listEvents() {
        std::lock_guard<std::mutex> lock(mutex_); // Lock for thread safety
        for (const auto &event : events_) {
            std::cout << "Event: " << event.getName() << ", Date: " << event.getDate() << std::endl;
        }
    }

private:
    std::vector<Event> events_;
    std::mutex mutex_; // Mutex for thread safety
};

// Function for thread
void registerEvents(EventManager &manager, const std::vector<std::pair<std::string, std::string>> &eventList) {
    for (const auto &event : eventList) {
        manager.registerEvent(event.first, event.second);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
    }
}

int main() {
    EventManager manager;

    // Example event lists to register
    std::vector<std::pair<std::string, std::string>> eventsA = {
        {"Concert", "2023-12-01"},
        {"Art Exhibition", "2023-11-15"},
    };

    std::vector<std::pair<std::string, std::string>> eventsB = {
        {"Tech Conference", "2023-10-22"},
        {"Food Festival", "2023-12-10"},
    };

    // Create two threads to register events concurrently
    std::thread threadA(registerEvents, std::ref(manager), eventsA);
    std::thread threadB(registerEvents, std::ref(manager), eventsB);

    // Wait for threads to finish
    threadA.join();
    threadB.join();

    // List all registered events
    std::cout << "\nAll Registered Events:" << std::endl;
    manager.listEvents();

    return 0;
}