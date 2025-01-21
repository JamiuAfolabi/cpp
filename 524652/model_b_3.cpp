#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>

// Severity level enum
enum class Severity {
    ERROR,
    WARNING,
    INFO
};

class SeverityLevelManager {
public:
    static Severity minLogLevel;

    static void setMinLogLevel(Severity level) {
        minLogLevel = level;
    }
};

SeverityLevelManager::Severity minLogLevel = Severity::INFO;

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
        return generateRandomValue(60, 100);
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
        return generateRandomValue(80, 120);
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
        return generateRandomValue(90, 100);
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
        return generateRandomValue(36.5, 37.5);
    }

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// DebugLogger Class with severity levels
class DebugLogger {
public:
    static void logSensorReading(const std::shared_ptr<Sensor>& sensor, Severity level = Severity::INFO) {
        if (level >= minLogLevel) {
            // Get current timestamp
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::tm buf;
            localtime_r(&in_time_t, &buf);

            // Format timestamp and severity
            std::cout << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << " | ";
            switch (level) {
                case Severity::ERROR:
                    std::cout << "[ERROR] ";
                    break;
                case Severity::WARNING:
                    std::cout << "[WARNING] ";
                    break;
                case Severity::INFO:
                    std::cout << "[INFO] ";
                    break;
            }

            std::string type = sensor->getType();
            float value = sensor->getValue();

            std::cout << type << ": " << value;

            // Checking for abnormal readings and log accordingly
            if (type == "Heart Rate" && (value < 60 || value > 100)) {
                std::cout << " | Abnormal Heart Rate!";
            } else if (type == "Blood Pressure" && (value < 90 || value > 120)) {
                std::cout << " | Abnormal Blood Pressure!";
            } else if (type == "Oxygen Level" && (value < 90)) {
                std::cout << " | Low Oxygen Level!";
            } else if (type == "Temperature" && (value < 36.5 || value > 37.5)) {
                std::cout << " | Abnormal Temperature!";
            }
            std::cout << "\n";
        }
    }
};

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

    // Example: Set minimum log level to WARNING to filter INFO messages
    SeverityLevelManager::setMinLogLevel(Severity::WARNING);

    std::cout << "\nReadings after setting minimum log level to WARNING:\n";
    monitoringSystem.displayReadings();

    return 0;
}