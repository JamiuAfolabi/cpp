#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>

// Base class for all sensors
class Sensor {
public:
    virtual ~Sensor() {}
    virtual std::string getType() const = 0;
    virtual float getValue() const = 0;
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

private:
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

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// DebugLogger Class for logging sensor data with timestamps
class DebugLogger {
public:
    static void logSensorReading(const std::shared_ptr<Sensor>& sensor) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        
        // Use localtime_r for macOS and Unix-based systems
        localtime_r(&in_time_t, &buf); // Use localtime_r instead of localtime_s

        // Format timestamp
        std::cout << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << " | ";

        std::string type = sensor->getType();
        float value = sensor->getValue();

        std::cout << type << ": " << value;

        // Checking for abnormal readings
        if (type == "Heart Rate" && (value < 60 || value > 100)) {
            std::cout << " | Abnormal Heart Rate!";
        }
        else if (type == "Blood Pressure" && (value < 90 || value > 120)) {
            std::cout << " | Abnormal Blood Pressure!";
        }
        else if (type == "Oxygen Level" && (value < 90)) {
            std::cout << " | Low Oxygen Level!";
        }
        else if (type == "Temperature" && (value < 36.5 || value > 37.5)) {
            std::cout << " | Abnormal Temperature!";
        }
        std::cout << "\n";
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
            DebugLogger::logSensorReading(sensor);
        }
    }

private:
    std::vector<std::shared_ptr<Sensor>> sensors;
};

int main() {
    PatientMonitoringSystem monitoringSystem;

    monitoringSystem.addSensor(std::make_shared<HeartRateSensor>());
    monitoringSystem.addSensor(std::make_shared<BloodPressureSensor>());
    monitoringSystem.addSensor(std::make_shared<OxygenLevelSensor>());
    monitoringSystem.addSensor(std::make_shared<TemperatureSensor>());

    // Simulate reading vital signs
    monitoringSystem.displayReadings();

    return 0;
}
