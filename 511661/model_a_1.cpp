#include <iostream>
#include <random>
#include <thread>
#include <chrono>

class ConnectionParameters {
public:
    float signalStrength; // in dBm
    bool connectionStatus; // true if connected, false otherwise
    float latency; // in ms
    float bandwidth; // in Mbps
    float packetLoss; // in percentage
    bool connectionTimeout; // true if timeout occurred

    void display() {
        std::cout << "Signal Strength: " << signalStrength << " dBm\n";
        std::cout << "Connection Status: " << (connectionStatus ? "Connected" : "Disconnected") << "\n";
        std::cout << "Latency: " << latency << " ms\n";
        std::cout << "Bandwidth: " << bandwidth << " Mbps\n";
        std::cout << "Packet Loss: " << packetLoss << " %\n";
        std::cout << "Connection Timeout: " << (connectionTimeout ? "Yes" : "No") << "\n";
    }
};

class NetworkTower {
private:
    std::random_device rd;
    std::mt19937 gen;

    float getRandomSignalStrength() {
        std::uniform_real_distribution<float> dist(-100.0f, -30.0f); // dBm
        return dist(gen);
    }

    float getRandomLatency() {
        std::uniform_real_distribution<float> dist(10.0f, 100.0f); // ms
        return dist(gen);
    }

    float getRandomBandwidth() {
        std::uniform_real_distribution<float> dist(1.0f, 100.0f); // Mbps
        return dist(gen);
    }

    float getRandomPacketLoss() {
        std::uniform_real_distribution<float> dist(0.0f, 10.0f); // %
        return dist(gen);
    }

public:
    NetworkTower() : gen(rd()) {}

    ConnectionParameters connect() {
        ConnectionParameters params;
        params.signalStrength = getRandomSignalStrength();
        params.latency = getRandomLatency();
        params.bandwidth = getRandomBandwidth();
        params.packetLoss = getRandomPacketLoss();
        params.connectionTimeout = (std::uniform_int_distribution<int>(0, 1)(gen) == 1);
        params.connectionStatus = !params.connectionTimeout; // Status is connected if no timeout
        return params;
    }
};

class ConnectionSimulator {
private:
    NetworkTower tower;
    int maxRetries;

public:
    ConnectionSimulator(int retries) : maxRetries(retries) {}

    void simulateConnection() {
        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            std::cout << "Attempt " << attempt << ":\n";
            ConnectionParameters params = tower.connect();
            params.display();
            std::cout << "-------------------------------------\n";

            // Simulate waiting time before the next retry if not connected
            if (!params.connectionStatus) {
                std::cout << "Retrying...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2)); // simulate wait time
            } else {
                break; // exit loop if connected
            }
        }
    }
};

int main() {
    int maxRetries = 5;
    ConnectionSimulator simulator(maxRetries);
    simulator.simulateConnection();
    return 0;
}