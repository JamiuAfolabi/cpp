#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class Service {
    DineIn = 1,
    Takeaway,
    Delivery,
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

// Subject that manages subscribers and notifies them
class RestaurantSystem {
public:
    void addSubscriber(std::shared_ptr<ISubscriber> subscriber) {
        subscribers.push_back(subscriber);
    }

    void toggleService(Service service, bool isEnabled) {
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
            default:
                break;
        }
    }

    void showCurrentServices() {
        std::cout << "Current Service Status:\n";
        std::cout << "Dine-In: " << (dineInEnabled ? "Enabled" : "Disabled") << "\n";
        std::cout << "Takeaway: " << (takeawayEnabled ? "Enabled" : "Disabled") << "\n";
        std::cout << "Delivery: " << (deliveryEnabled ? "Enabled" : "Disabled") << "\n";
    }

private:
    bool dineInEnabled = false;
    bool takeawayEnabled = false;
    bool deliveryEnabled = false;
    std::vector<std::shared_ptr<ISubscriber>> subscribers;

    void notifySubscribers(const std::string& message) {
        for (const auto& subscriber : subscribers) {
            subscriber->notify(message);
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
        std::cout << "4. Show Current Service Status\n";
        std::cout << "5. Exit\n";
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
                restaurant.showCurrentServices();
                break;
            case 5:
                std::cout << "Exiting the system." << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Please select again." << std::endl;
                break;
        }
    }

    return 0;
}