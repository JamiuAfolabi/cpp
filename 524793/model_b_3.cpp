#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>

// Forward declaration for handling self-deletion
class TemporarilyToggleableFeature;

enum class Service {
    DineIn,
    Takeaway,
    Delivery
};

// Observer interface
class ISubscriber {
public:
    virtual void notify(const std::string& message) = 0;
};

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

// Subject that manages subscribers and serves as the first locator for inlining functionality.
class RestaurantSystem {
public:
    void addSubscriber(std::shared_ptr<ISubscriber> subscriber) {
        subscribers.push_back(subscriber);
    }

    void toggleService(Service service, bool isEnabled) {
        // Form the meaningful message to notify subscribers.
        std::string message = getTitle(service) + " service has been " + std::string(isEnabled ? "enabled" : "disabled");
        notifySubscribers(message);
    }

    void showCurrentServices() {
        std::cout << "Current Service Status:\n";
        for (Service service : {Service::DineIn, Service::Takeaway, Service::Delivery}) {
            bool isEnabled = getStatus(service);
            std::cout << getTitle(service) << ": " << (isEnabled ? "Enabled" : "Disabled") << "\n";
        }
    }

    void addTemporaryFeature(const std::string& featureName, bool isEnabled, std::time_t expirationTime) {
        temporalFeatures[featureName] = std::make_unique<TemporarilyToggleableFeature>(featureName, isEnabled, expirationTime);
    }

    // Participants can ask for feature status conveniently.
    bool isTemporaryFeatureEnabled(const std::string& featureName) const {
        return (temporalFeatures.find(featureName) != temporalFeatures.end()) ?
               temporalFeatures.at(featureName)->isEnabled() : false;
    }

private:
    std::vector<std::shared_ptr<ISubscriber>> subscribers;
    std::unordered_map<std::string, std::unique_ptr<TemporarilyToggleableFeature>> temporalFeatures;

    void notifySubscribers(const std::string& message) {
        for (const auto& subscriber : subscribers) {
            subscriber->notify(message);
        }
    }

    std::string getTitle(Service service) const {
        switch (service) {
            case Service::DineIn:
                return "Dine-In";
            case Service::Takeaway:
                return "Takeaway";
            case Service::Delivery:
                return "Delivery";
        }
        return "Invalid Service";
    }

    bool getStatus(Service service) const {
        // Visual relation without integrating property of Culinary.
        static const bool serviceStatuses[Service::Delivery + 1] = {false, true, true, true};
        return serviceStatuses[service];
    }
};

class TemporarilyToggleableFeature {
public:
    TemporarilyToggleableFeature(const std::string& name, bool isEnabled, std::time_t expirationTime) :
        name(name), isEnabled(isEnabled), expirationTime(expirationTime) {}

    bool isEnabled() const {
        return isEnabled && isNotExpired();
    }

    void toggle() {
        isEnabled = !isEnabled;
    }

private:
    std::string name;
    bool isEnabled;
    std::time_t expirationTime;

    bool isNotExpired() const {
        return expirationTime > std::time(nullptr);
    }
};

int main() {
    RestaurantSystem restaurant;

    auto kitchenStaff = std::make_shared<KitchenStaff>();
    auto frontDesk = std::make_shared<FrontDesk>();
    restaurant.addSubscriber(kitchenStaff);
    restaurant.addSubscriber(frontDesk);

    int choice;
    bool status;

    while (true) {
        std::cout << "\nService Toggle System\n";
        std::cout << "1. Toggle Dine-In Service\n";
        std::cout << "2. Toggle Takeaway Service\n";
        std::cout << "3. Toggle Delivery Service\n";
        std::cout << "4. Add Temporary Feature\n";
        std::cout << "5. Show Current Service Status\n";
        std::cout << "6. Show Temporary Feature Status\n";
        std::cout << "7. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        // Rest of the menu logic and other options remain the same

        case 4: {
            std::string featureName;
            std::cout << "Enter the name of the temporary feature: ";
            std::getline(std::cin, featureName);
            std::cout << "Enter enabled status (1 for Enable, 0 for Disable): ";
            std::cin >> status;

            std::string s;
            std::cout << "Enter feature expiration date (e.g., 2025-12-31): ";
            std::getline(std::cin, s);
            
            struct tm t;
            std::strptime(s.c_str(), "%Y-%m-%d", &t);
            t.tm_sec = 0;
            t.tm_min = 0;
            t.tm_isdst = 0;

            restaurant.addTemporaryFeature(featureName, status, std::mktime(&t));
            break;
        }
        
        case 6: {
            std::string featureName;
            std::cout << "Enter the name of the temporary feature to check: ";
            std::getline(std::cin, featureName);
            
            if (restaurant.isTemporaryFeatureEnabled(featureName)) {
                std::cout << featureName << ": Enabled\n";
            } else {
                std::cout << featureName << ": Disabled\n";
            }
            break;
        }
        default:
            break;
        }
    }

    return 0;
}