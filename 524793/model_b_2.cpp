#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum class Service {
    DineIn = 1,
    Takeaway,
    Delivery,
    Exit
};

class Observer {
protected:
    std::string name;

public:
    Observer(const std::string& name) : name(name) {}
    virtual ~Observer() = default;
    virtual void update() = 0;
    std::string getName() const { return name; }
};

class KitchenStaff : public Observer {
public:
    KitchenStaff() : Observer("Kitchen Staff") {}

    void update() override {
        std::cout << "**" << getName() << " notified:** Service change has occurred." << std::endl;
    }
};

class FrontDesk : public Observer {
public:
    FrontDesk() : Observer("Front Desk") {}

    void update() override {
        std::cout << "**" << getName() << " notified:** Service change has occurred." << std::endl;
    }
};

class RestaurantSystem {
public:
    void registerObserver(Observer* observer) {
        observers.push_back(observer);
    }
    
    void removeObserver(const std::string& observerName) {
        observers.erase(std::remove_if(observers.begin(), observers.end(), [&](Observer* observer) {
            return observer->getName() == observerName;
        }), observers.end());
    }

    void toggleService(Service service, bool isEnabled) {
        // ... (Same code as before)

        notifyAllObservers();
    }

    void showCurrentServices() {
        // ... (Same code as before)
    }

private:
    std::vector<Observer*> observers;

    void notifyAllObservers() {
        for (auto observer : observers) {
            observer->update();
        }
    }
};

int main() {
    RestaurantSystem restaurant;
    int choice;
    bool status;
    KitchenStaff kitchenStaff;
    FrontDesk frontDesk;

    restaurant.registerObserver(&kitchenStaff);
    restaurant.registerObserver(&frontDesk);

    // ... (Rest of the code remains the same)
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

    return 0;
}