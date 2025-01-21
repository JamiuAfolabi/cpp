#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>

// Severity Levels for Logging
enum class LogLevel {
    ERROR,
    WARNING,
    INFO
};

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
        return generateRandomValue(50, 120); // random heart rate between 50 and 120 bpm
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
        return generateRandomValue(70, 130); // random systolic pressure
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
        return generateRandomValue(85, 100); // random oxygen saturation percentage
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
        return generateRandomValue(36.0, 38.0); // random temperature in Celsius
    }

private:
    float generateRandomValue(float min, float max) const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
};

// DebugLogger Class for logging sensor data with timestamps and severity levels
class DebugLogger {
public:
    DebugLogger(LogLevel level) : currentLogLevel(level) {}

    void logSensorReading(const std::shared_ptr<Sensor>& sensor) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        localtime_r(&in_time_t, &buf); // Use localtime_r for macOS and Unix-based systems

        // Log the reading with timestamp
        std::string logMessage;
        logMessage += "[" + severityToString(INFO) + "] " + std::put_time(&buf, "%Y-%m-%d %H:%M:%S") + " | ";

        std::string type = sensor->getType();
        float value = sensor->getValue();
        logMessage += type + ": " + std::to_string(value);

        // Checking for abnormal readings and setting severity levels
        if (type == "Heart Rate" && (value < 60 || value > 100)) {
            logMessage += " | " + severityToString(ERROR) + " Abnormal Heart Rate!";
            log(ERROR, logMessage);
        }
        else if (type == "Blood Pressure" && (value < 90 || value > 120)) {
            logMessage += " | " + severityToString(WARNING) + " Abnormal Blood Pressure!";
            log(WARNING, logMessage);
        }
        else if (type == "Oxygen Level" && (value < 90)) {
            logMessage += " | " + severityToString(WARNING) + " Low Oxygen Level!";
            log(WARNING, logMessage);
        }
        else if (type == "Temperature" && (value < 36.5 || value > 37.5)) {
            logMessage += " | " + severityToString(ERROR) + " Abnormal Temperature!";
            log(ERROR, logMessage);
        } else {
            log(INFO, logMessage);
        }
    }

    void setLogLevel(LogLevel level) {
        currentLogLevel = level;
    }

private:
    LogLevel currentLogLevel;

    void log(LogLevel level, const std::string& message) {
        if (level <= currentLogLevel) {
            std::cout << message << "\n"; // Print message if it meets the logging level
        }
    }

    std::string severityToString(LogLevel level) const {
        switch (level) {
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::INFO: return "INFO";
            default: return "UNKNOWN";
        }
    }
};

// Patient Monitoring System
class PatientMonitoringSystem {
public:
    PatientMonitoringSystem(LogLevel logLevel) : logger(logLevel) {}

    void addSensor(std::shared_ptr<Sensor> sensor) {
        sensors.push_back(sensor);
    }

    void displayReadings() const {
        std::cout << "Patient Vital Signs Readings:\n";
        for (const auto& sensor : sensors) {
            logger.logSensorReading(sensor);
        }
    }

private:
    std::vector<std::shared_ptr<Sensor>> sensors;
    DebugLogger logger;
};

int main() {
    PatientMonitoringSystem monitoringSystem(LogLevel::INFO); // Set the log level

    monitoringSystem.addSensor(std::make_shared<HeartRateSensor>());
    monitoringSystem.addSensor(std::make_shared<BloodPressureSensor>());
    monitoringSystem.addSensor(std::make_shared<OxygenLevelSensor>());
    monitoringSystem.addSensor(std::make_shared<TemperatureSensor>());

    // Simulate reading vital signs
    monitoringSystem.displayReadings();

    return 0;
}