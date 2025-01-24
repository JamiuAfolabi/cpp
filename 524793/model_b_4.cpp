#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <map>
#include <fstream>

enum class Service {
    DineIn = 1,
    Takeaway,
    Delivery,
    SeasonalMenu,
    Exit
};

// Observer interface
class ISubscriber {
public:
    virtual void notify(const std::string& message) = 0;
};

// Concrete Subscribers
class KitchenStaff : public ISubscriber {
public:
    void notify(const std::string& message) override {
        std::cout << "[Kitchen Staff Notification] " << message << std::endl;
    }
};

class FrontDesk : public ISubscriber {
public:
    void notify(const std::string& message) override {
        std::cout << "[Front Desk Notification] " << message << std::endl;
    }
};

// Class to manage temporary features
class TemporaryFeature {
public:
    TemporaryFeature(const std::string& name, int duration) 
        : name(name), duration(duration), active(true) {}

    void checkExpiration() {
        if (active && (std::chrono::steady_clock::now() - start_time) >= std::chrono::seconds(duration)) {
            active = false;
            std::cout << name << " has been automatically disabled after " << duration << " seconds." << std::endl;
        }
    }

    void activate() {
        start_time = std::chrono::steady_clock::now();
        active = true;
        std::cout << name << " has been enabled for " << duration << " seconds." << std::endl;
    }

    bool isActive() const {
        return active;
    }

private:
    std::string name;
    int duration; // duration in seconds
    bool active;
    std::chrono::steady_clock::time_point start_time;
};

// Subject that manages subscribers and notifies them
class RestaurantSystem {
public:
    RestaurantSystem() {
        toggleCount[Service::DineIn] = 0;
        toggleCount[Service::Takeaway] = 0;
        toggleCount[Service::Delivery] = 0;
        toggleCount[Service::SeasonalMenu] = 0;
    }

    void addSubscriber(std::shared_ptr<ISubscriber> subscriber) {
        subscribers.push_back(subscriber);
    }

    void toggleService(Service service, bool isEnabled) {
        toggleCount[service]++;
        switch (service) {
            case Service::DineIn:
                dineInEnabled = isEnabled;
                notifySubscribers("Dine-In service has been " + std::string(isEnabled ? "enabled" : "disabled"));
                break;
            case Service::Takeaway:
                takeawayEnabled = isEnabled;
                notifySubscribers("Takeaway service has been " + std::string(isEnabled ? "enabled" : "disabled"));
                break;
            case Service::Delivery:
                deliveryEnabled = isEnabled;
                notifySubscribers("Delivery service has been " + std::string(isEnabled ? "enabled" : "disabled"));
                break;
            case Service::SeasonalMenu:
                if (isEnabled) {
                    temporaryFeatures.emplace_back(std::make_shared<TemporaryFeature>("Seasonal Menu", 10)); // Active for 10 seconds
                    temporaryFeatures.back()->activate();
                    notifySubscribers("Seasonal Menu has been enabled.");
                }
                break;
            default:
                break;
        }
    }

    void showCurrentServices() {
        std::cout << "Current Service Status:\n";
        std::cout << "Dine-In: " << (dineInEnabled ? "Enabled" : "Disabled") << "\n";
        std::cout << "Takeaway: " << (takeawayEnabled ? "Enabled" : "Disabled") << "\n";
        std::cout << "Delivery: " << (deliveryEnabled ? "Enabled" : "Disabled") << "\n";
        std::cout << "Seasonal Menu: " << (isSeasonalMenuActive() ? "Enabled" : "Disabled") << "\n";
    }

    void logToggleFrequencies() {
        std::ofstream logFile("service_toggles.log");
        if (logFile.is_open()) {
            logFile << "Service Toggle Frequencies:\n";
            for (const auto& entry : toggleCount) {
                logFile << ServiceName(entry.first) << ": " << entry.second << " times\n";
            }
            logFile.close();
            std::cout << "Toggle frequencies logged to 'service_toggles.log'." << std::endl;
        } else {
            std::cout << "Error: Could not open log file." << std::endl;
        }
    }

    void updateFeatures() {
        for (auto& feature : temporaryFeatures) {
            feature->checkExpiration();
        }
    }

private:
    bool dineInEnabled = false;
    bool takeawayEnabled = false;
    bool deliveryEnabled = false;
    std::vector<std::shared_ptr<ISubscriber>> subscribers;
    std::vector<std::shared_ptr<TemporaryFeature>> temporaryFeatures;
    std::map<Service, int> toggleCount;

    void notifySubscribers(const std::string& message) {
        for (const auto& subscriber : subscribers) {
            subscriber->notify(message);
        }
    }

    bool isSeasonalMenuActive() {
        for (const auto& feature : temporaryFeatures) {
            if (feature->isActive()) {
                return true;
            }
        }
        return false;
    }

    std::string ServiceName(Service service) {
        switch (service) {
            case Service::DineIn:
                return "Dine-In";
            case Service::Takeaway:
                return "Takeaway";
            case Service::Delivery:
                return "Delivery";
            case Service::SeasonalMenu:
                return "Seasonal Menu";
            default:
                return "";
        }
    }
};

int main() {
    RestaurantSystem restaurant;

    // Creating subscribers
    auto kitchenStaff = std::make_shared<KitchenStaff>();
    auto frontDesk = std::make_shared<FrontDesk>();

    // Adding subscribers to the system
    restaurant.addSubscriber(kitchenStaff);
    restaurant.addSubscriber(frontDesk);

    int choice;
    bool status;

    while (true) {
        std::cout << "\nService Toggle System\n";
        std::cout << "1. Toggle Dine-In Service\n";
        std::cout << "2. Toggle Takeaway Service\n";
        std::cout << "3. Toggle Delivery Service\n";
        std::cout << "4. Enable Seasonal Menu\n";
        std::cout << "5. Show Current Service Status\n";
        std::cout << "6. Log Toggle Frequencies\n";
        std::cout << "7. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enable or Disable Dine-In Service? (1 for Enable, 0 for Disable): ";
                std::cin >> status;
                restaurant.toggleService(Service::DineIn, status);
                break;
            case 2:
                std::cout << "Enable or Disable Takeaway Service? (1 for Enable, 0 for Disable): ";
                std::cin >> status;
                restaurant.toggleService(Service::Takeaway, status);
                break;
            case 3:
                std::cout << "Enable or Disable Delivery Service? (1 for Enable, 0 for Disable): ";
                std::cin >> status;
                restaurant.toggleService(Service::Delivery, status);
                break;
            case 4:
                restaurant.toggleService(Service::SeasonalMenu, true);
                break;
            case 5:
                restaurant.showCurrentServices();
                break;
            case 6:
                restaurant.logToggleFrequencies();
                break;
            case 7:
                std::cout << "Exiting the system." << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Please select again." << std::endl;
                break;
        }

        restaurant.updateFeatures();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second to simulate time passage
    }

    return 0;
}
