#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>

// Base class for all sensors
class Sensor {
public:
    virtual ~Sensor() {}
    virtual std::string getType() const = 0;
    virtual float getValue() const = 0;
    virtual bool isOperational() const = 0; // To check if the sensor is working
};

// Heart Rate Sensor
class HeartRateSensor : public Sensor {
public:
    HeartRateSensor() {}

    std::string getType() const override {
        return "Heart Rate";
    }

    float getValue() const override {
        return generateRandomValue(60, 100); // random heart rate between 60 and 100 bpm
    }

    bool isOperational() const override {
        return true; // Simulate that the sensor is operational
    }

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// Blood Pressure Sensor
class BloodPressureSensor : public Sensor {
public:
    BloodPressureSensor() {}

    std::string getType() const override {
        return "Blood Pressure";
    }

    float getValue() const override {
        return generateRandomValue(80, 120); // random systolic pressure
    }

    bool isOperational() const override {
        return true; // Simulate that the sensor is operational
    }

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// Oxygen Level Sensor
class OxygenLevelSensor : public Sensor {
public:
    OxygenLevelSensor() {}

    std::string getType() const override {
        return "Oxygen Level";
    }

    float getValue() const override {
        return generateRandomValue(90, 100); // random oxygen saturation percentage
    }

    bool isOperational() const override {
        return isOperationalStatus; // Simulate operational status
    }

    void setOperationalStatus(bool status) {
        isOperationalStatus = status; // Control the operational status for testing
    }

private:
    bool isOperationalStatus = true;
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// Temperature Sensor
class TemperatureSensor : public Sensor {
public:
    TemperatureSensor() {}

    std::string getType() const override {
        return "Temperature";
    }

    float getValue() const override {
        return generateRandomValue(36.5, 37.5); // random temperature in Celsius
    }

    bool isOperational() const override {
        return true; // Simulate that the sensor is operational
    }

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// Patient Monitoring System
class PatientMonitoringSystem {
public:
    void addSensor(std::shared_ptr<Sensor> sensor) {
        sensors.push_back(sensor);
    }

    void displayReadings() const {
        std::cout << "Patient Vital Signs Readings:\n";
        for (const auto& sensor : sensors) {
            std::cout << "[" << getCurrentTime() << "] "
                      << sensor->getType() << ": "
                      << sensor->getValue() << " "
                      << (sensor->isOperational() ? "(Operational)" : "(Faulty)") << "\n";
        }
    }

private:
    std::vector<std::shared_ptr<Sensor>> sensors;

    std::string getCurrentTime() const {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

// Main function
int main() {
    PatientMonitoringSystem monitoringSystem;

    // Creating sensors and adding them to the monitoring system
    monitoringSystem.addSensor(std::make_shared<HeartRateSensor>());
    monitoringSystem.addSensor(std::make_shared<BloodPressureSensor>());
    
    auto oxygenLevelSensor = std::make_shared<OxygenLevelSensor>();
    oxygenLevelSensor->setOperationalStatus(true); // Set operational status
    monitoringSystem.addSensor(oxygenLevelSensor);

    auto temperatureSensor = std::make_shared<TemperatureSensor>();
    temperatureSensor->setOperationalStatus(false); // Simulate a faulty sensor
    monitoringSystem.addSensor(temperatureSensor);

    // Simulate reading vital signs
    monitoringSystem.displayReadings();

    return 0;
}