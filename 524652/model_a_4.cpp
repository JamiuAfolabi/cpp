#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>

// Severity levels for the logger
enum class Severity {
    ERROR,
    WARNING,
    INFO
};

// Logger interface
class ILogger {
public:
    virtual ~ILogger() {}
    virtual void log(const std::string& message) = 0;
};

// Console Logger
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

// File Logger
class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename) : logFile(filename) {
        if (!logFile.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
    }

    void log(const std::string& message) override {
        logFile << message << std::endl;
    }

    ~FileLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

private:
    std::ofstream logFile;
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

// DebugLogger Class for logging sensor data with timestamps and severity levels
class DebugLogger {
public:
    DebugLogger(std::unique_ptr<ILogger> logger) : logger(std::move(logger)) {}

    static Severity currentSeverityLevel; // Global variable to set the severity level filter

    void logSensorReading(const std::shared_ptr<Sensor>& sensor, Severity severity) {
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
        std::ostringstream oss;
        oss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << " | ";

        std::string type = sensor->getType();
        float value = sensor->getValue();

        // Log based on severity
        std::string severityStr = severity == Severity::ERROR ? "[ERROR]" :
                                  severity == Severity::WARNING ? "[WARNING]" : "[INFO]";
        oss << severityStr << " | ";

        oss << type << ": " << value;

        // Checking for abnormal readings and assigning severity
        if (type == "Heart Rate" && (value < 60 || value > 100)) {
            oss << " | Abnormal Heart Rate!";
            severity = Severity::ERROR;
        }
        else if (type == "Blood Pressure" && (value < 90 || value > 120)) {
            oss << " | Abnormal Blood Pressure!";
            severity = Severity::ERROR;
        }
        else if (type == "Oxygen Level" && (value < 90)) {
            oss << " | Low Oxygen Level!";
            severity = Severity::WARNING;
        }
        else if (type == "Temperature" && (value < 36.5 || value > 37.5)) {
            oss << " | Abnormal Temperature!";
            severity = Severity::WARNING;
        }

        logger->log(oss.str());
    }

private:
    std::unique_ptr<ILogger> logger;
};

// Set the default severity level to INFO
Severity DebugLogger::currentSeverityLevel = Severity::INFO;

// Patient Monitoring System
class PatientMonitoringSystem {
public:
    void addSensor(std::shared_ptr<Sensor> sensor) {
        sensors.push_back(sensor);
    }

    void displayReadings(DebugLogger& logger) const {
        std::cout << "Patient Vital Signs Readings:\n";
        for (const auto& sensor : sensors) {
            logger.logSensorReading(sensor, Severity::INFO); // Change to other severity levels if necessary
        }
    }

private:
    std::vector<std::shared_ptr<Sensor>> sensors;
};

int main() {
    // Choose your logger type here
    std::unique_ptr<ILogger> logger = std::make_unique<ConsoleLogger>();
    // To log to a file, use the line below instead:
    // std::unique_ptr<ILogger> logger = std::make_unique<FileLogger>("log.txt");

    DebugLogger debugLogger(std::move(logger));

    PatientMonitoringSystem monitoringSystem;
    monitoringSystem.addSensor(std::make_shared<HeartRateSensor>());
    monitoringSystem.addSensor(std::make_shared<BloodPressureSensor>());
    monitoringSystem.addSensor(std::make_shared<OxygenLevelSensor>());
    monitoringSystem.addSensor(std::make_shared<TemperatureSensor>());

    // Set severity filter (INFO, WARNING, ERROR)
    DebugLogger::currentSeverityLevel = Severity::INFO;

    // Simulate reading vital signs
    monitoringSystem.displayReadings(debugLogger);

    return 0;
}