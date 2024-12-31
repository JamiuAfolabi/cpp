
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <condition_variable>

class Event {
public:
    std::string name;
    Event(const std::string& name) : name(name) {}
};

class EventManager {
private:
    std::vector<Event> events;
    mutable std::mutex mtx;

public:
    void registerEvent(const Event& event) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            events.push_back(event);
        }
        newEventCondVar.notify_one();
    }
    
    // Method to retrieve all registered events
    std::vector<Event> getRegisteredEvents() {
        std::vector<Event> result;
        {
            std::unique_lock<std::mutex> lock(mtx);
            result = events;
        }
        return result;
    }
    
    // Variable to wait for new event registration.
    std::condition_variable newEventCondVar;
};

// Thread function for event registration
void registerEventThread(EventManager& eventManager, const std::string& eventName) {
    Event event(eventName);
    eventManager.registerEvent(event);
}

int main() {
    EventManager eventManager;
    std::thread thread1(registerEventThread, std::ref(eventManager), "Event 1");
    std::thread thread2(registerEventThread, std::ref(eventManager), "Event 2");

    //Simulate other tasks while waiting for events to be registered
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    // Wait for both threads to complete
    thread1.join();
    thread2.join();

    // Get all registered events after thread completion
    auto registeredEvents = eventManager.getRegisteredEvents();

    std::cout << "Registered Events:" << std::endl;
    for (const auto& event : registeredEvents) {
        std::cout << "  - " << event.name << std::endl;
    }

    return 0;
}
