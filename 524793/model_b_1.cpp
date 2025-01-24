#include <iostream>
#include <string>
#include <vector>

enum class Service {
    DINE_IN,
    TAKEAWAY,
    DELIVERY
};

enum class TimeOfDay {
    MORNING,
    AFTERNOON,
    EVENING,
    NIGHT
};

class ServiceToggle {
private:
    bool staffAvailable;
    std::vector<Service> availableServices;

public:
    ServiceToggle(bool staffAvailable) : staffAvailable(staffAvailable) {}

    void toggleService(Service service) {
        if (std::find(availableServices.begin(), availableServices.end(), service) != availableServices.end()) {
            // Service is already enabled, disable it
            availableServices.erase(std::remove(availableServices.begin(), availableServices.end(), service), availableServices.end());
        } else {
            // Service is disabled, enable it
            availableServices.push_back(service);
        }
    }
    
    void updateServices(TimeOfDay timeOfDay) {
        availableServices.clear();
        switch (timeOfDay) {
            case TimeOfDay::MORNING:
                toggleService(Service::TAKEAWAY);
                if (staffAvailable) {
                    toggleService(Service::DINE_IN);
                }
                break;
            case TimeOfDay::AFTERNOON:
                toggleService(Service::TAKEAWAY);
                toggleService(Service::DELIVERY);
                if (staffAvailable) {
                    toggleService(Service::DINE_IN);
                }
                break;
            case TimeOfDay::EVENING:
                toggleService(Service::DINE_IN);
                toggleService(Service::DELIVERY);
                break;
            case TimeOfDay::NIGHT:
                break;
        }
    }

    void notifyStaff() const {
        std::cout << "Service Update Notification to Staff:\n";
        if (availableServices.empty()) {
            std::cout << "All services are closed due to staff unavailability or off-peak hours.\n";
        } else {
            std::cout << "The following services are available:\n";
            for (const Service& service : availableServices) {
                switch (service) {
                    case Service::DINE_IN:
                        std::cout << "Dine-in\n";
                        break;
                    case Service::TAKEAWAY:
                        std::cout << "Takeaway\n";
                        break;
                    case Service::DELIVERY:
                        std::cout << "Delivery\n";
                        break;
                }
            }
        }
    }
};

int main() {
    ServiceToggle toggleSystem(true); // Assuming staff is available initially

    std::cout << "Initial Services:";
    toggleSystem.notifyStaff();

    toggleSystem.updateServices(TimeOfDay::AFTERNOON);
    std::cout << "\nServices after updating to AFTERNOON:";
    toggleSystem.notifyStaff();

    toggleSystem.toggleService(Service::DELIVERY);
    std::cout << "\nServices after disabling DELIVERY:";
    toggleSystem.notifyStaff();

    toggleSystem.updateServices(TimeOfDay::NIGHT);
    std::cout << "\nServices after updating to NIGHT:";
    toggleSystem.notifyStaff();

    toggleSystem.staffAvailable = false; // Simulate staff unavailability
    toggleSystem.updateServices(TimeOfDay::MORNING);
    std::cout << "\nServices after updating to MORNING with staff unavailability:";
    toggleSystem.notifyStaff();

    return 0;
}
