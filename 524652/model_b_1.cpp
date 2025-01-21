#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include <random>

// Interface for all Sensors
class Sensor {
public:
    virtual ~Sensor() = default;
    // Read data from sensor
    virtual double readData() = 0;

    std::string getName() const = 0;
};

class HeartRateSensor : public Sensor {
public:
    double readData() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(70.0, 10.0);
        // Simulate heart rate data using normal distribution (mu=70, sd=10)
        return dist(gen);
    }
    std::string getName() const override { return "Heart Rate Sensor"; }
};

class BloodPressureSensor : public Sensor {
public:
    double readData() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(120.0/60.0, 15.0/60.0); // systolic pressure /10, diastolic pressure/10
        double systolic_sd = dist(gen); 
        std::normal_distribution<> dist_dias(80.0/60.0, 10.0/60.0);
        double diastolic_sd = dist_dias(gen);
        return systolic_sd*10 + diastolic_sd*10; //signifying mL/mM Hg
    }
    std::string getName() const override { return "Blood Pressure Sensor"; }
};

class OxygenLevelSensor : public Sensor {
public:
    double readData() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(98.0, 3.0);
        // Simulate oxygen level data using normal distribution (mu=98, sd=3)
        return dist(gen);
    }
    std::string getName() const override { return "Oxygen Level Sensor"; }
};

class TemperatureSensor : public Sensor {
public:
    double readData() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(36.5, 0.3);
        // Simulate temperature data using normal distribution (mu=36.5, sd=0.3)
        return dist(gen);
    }
    std::string getName() const override { return "Temperature Sensor"; }
};

class PatientMonitoringSystem {
private:
    HeartRateSensor heartRateSensor;
    BloodPressureSensor bloodPressureSensor;
    OxygenLevelSensor oxygenLevelSensor;
    TemperatureSensor temperatureSensor;

public:
     void pollSensorsAndDisplay() {
         while (true) {
             {
                 std::scoped_lock lock(dataMutex);
                 std::cout << "*** Vital Signs Reading ***" << std::endl;
                 std::cout << heartRateSensor.getName()    << ": " << heartRateSensor.readData() << " bpm" << std::endl;
                 std::cout << bloodPressureSensor.getName() << ": " << bloodPressureSensor.readData() << " mL/mM Hg" << std::endl;
                 std::cout << oxygenLevelSensor.getName()  << ": " << oxygenLevelSensor.readData() << " %" << std::endl;
                 std::cout << temperatureSensor.getName()  << ": " << temperatureSensor.readData() << " \u00B0C" << std::endl;