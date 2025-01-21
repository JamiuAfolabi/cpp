#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>  // For file logging

// Enum for log destinations
enum class LogDestination {
    CONSOLE,
    FILE
};

// Severity levels for the logger
enum class Severity {
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

// Helper function for logging to the console
void logToConsole(const std::string& message) {
    std::cout << message << "\n";
}

// Helper function for logging to a file
void logToFile(const std::string& logFilePath, const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.close();
    } else {
        std::cerr << "Error opening log file: " << logFilePath << "\n";
    }
}

// DebugLogger Class for logging sensor data with timestamps, severity levels, and log destinations
class DebugLogger {
public:
    static Severity currentSeverityLevel; // Global variable to set the severity level filter
    static LogDestination currentLogDestination; // Global variable to set the log destination

    static void logSensorReading(const std::shared_ptr<Sensor>& sensor, Severity severity, LogDestination logDestination = currentLogDestination) {
        // Check if the severity level should be logged
        if (static_cast<int>(severity) > static_cast<int>(currentSeverityLevel)) {
            return; // Skip logging if the severity level is below the current global filter level
        }

        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;

        localtime_r(&in_time_t, &buf); // Use localtime_r for macOS and Unix-based systems

        // Format timestamp
        std::string message = std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << " | ";

        std::string type = sensor->getType();
        float value = sensor->getValue();

        // Log based on severity
        std::string severityStr = severity == Severity::ERROR ? "[ERROR]" :
                                  severity == Severity::WARNING ? "[WARNING]" : "[INFO]";
        message += severityStr << " | ";

        message += type << ": " << value;

        // Checking for abnormal readings and assigning severity
        if (type == "Heart Rate" && (value < 60 || value > 100)) {
            message += " | Abnormal Heart Rate!";
            severity = Severity::ERROR;
        }
        else if (type == "Blood Pressure" && (value < 90 || value > 120)) {
            message += " | Abnormal Blood Pressure!";
            severity = Severity::ERROR;
        }
        else if (type == "Oxygen Level" && (value < 90)) {
            message += " | Low Oxygen Level!";
            severity = Severity::WARNING;
        }
        else if (type == "Temperature" && (value < 36.5 || value > 37.5)) {
            message += " | Abnormal Temperature!";
            severity = Severity::WARNING;
        }

        switch (logDestination) {
            case LogDestination::CONSOLE:
                logToConsole(message);
                break;
            case LogDestination::FILE:
                logToFile("patient_logs.txt", message); // You can change the log file path here
                break;
        }
    }
};

// Set the default severity level to INFO and log destination to CONSOLE
Severity DebugLogger::currentSeverityLevel = Severity::INFO;
LogDestination DebugLogger::currentLogDestination = LogDestination::CONSOLE;

// Patient Monitoring System
class PatientMonitoringSystem {
public:
    void addSensor(std::shared_ptr<Sensor> sensor) {
        sensors.push_back(sensor);
    }

    void displayReadings(LogDestination logDestination = DebugLogger::currentLogDestination) const {
        std::cout << "Patient Vital Signs Readings:\n";
        for (const auto& sensor : sensors) {
            DebugLogger::logSensorReading(sensor, Severity::INFO, logDestination);
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