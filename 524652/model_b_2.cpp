#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>

// Base class for all sensors
class Sensor {
public:
    virtual ~Sensor() {}
    virtual std::string getType() const = 0;
    virtual float getValue() const = 0;
    virtual bool isConnected() const = 0;
    void setConnected(bool connected);

private:
    bool connected = true;
};

// Helper function for timestamp
std::string getCurrentTimestamp() {
    time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    return std::string()
         << std::put_time(tm, "%Y-%m-%d %H:%M:%S.%f");
}


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

class DebugLogger {
public:
    void log(const std::string& message, const std::string& timestamp = "") const {
        if (!timestamp.empty()) {
            std::cout << "[DEBUG - " << timestamp << "] " << message << "\n";
        } else {
            std::cout << "[DEBUG] " << message << "\n";
        }
    }

    void sensorReading(const Sensor& sensor, float value, const std::string& timestamp) const {
        log(f"{getTypePrefix(sensor)} {sensor.getType()} Reading: {value:.2f}  ({' ' if sensor.isConnected() else '*'}Connected)", timestamp);
    }

    std::string getTypePrefix(const Sensor& sensor) const {
        return sensor.isConnected() ? "OK" : "ERR";
    }
};


class PatientMonitoringSystem {
public:
    PatientMonitoringSystem(DebugLogger& logger) : logger(logger) {}

    void addSensor(std::shared_ptr<Sensor> sensor) {
        sensors.push_back(sensor);
        logger.log(f"Sensor {sensor->getType()} added");
    }

    void startReadingLoop() {
        std::thread readingThread([this] {
            while (true) {
                readSensorsAndLog();
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate every-second readings
            }
        });
        readingThread.detach();
    }

private:
    void readSensorsAndLog() {
        for (const auto& sensor : sensors) {
            float value = sensor->getValue();
            logger.sensorReading(*sensor, value, getCurrentTimestamp());
        }
    }

    void displayReadings() const {
        // Same as before
    }

private:
    DebugLogger logger;
    std::vector<std::shared_ptr<Sensor>> sensors;
};

int main() {
    DebugLogger debugLogger;

    PatientMonitoringSystem monitoringSystem(debugLogger);

    monitoringSystem.addSensor(std::make_shared<HeartRateSensor>());
    monitoringSystem.addSensor(std::make_shared<BloodPressureSensor>());
    monitoringSystem.addSensor(std::make_shared<OxygenLevelSensor>());
    monitoringSystem.addSensor(std::make_shared<TemperatureSensor>());

    monitoringSystem.startReadingLoop();

    std::cout << "Patient Monitoring System started. Ctrl-C to exit.\n";
    // Let the monitoring system run, press Ctrl-C to exit
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}