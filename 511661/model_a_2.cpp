#include <iostream>
#include <random>
#include <thread>
#include <chrono>

// Define global constants
#define MAX_SIGNAL_STRENGTH -30.0f // in dBm
#define MIN_SIGNAL_STRENGTH -100.0f // in dBm
#define MAX_LATENCY 100.0f // in ms
#define MIN_LATENCY 10.0f // in ms
#define MAX_BANDWIDTH 100.0f // in Mbps
#define MIN_BANDWIDTH 1.0f // in Mbps
#define MAX_PACKET_LOSS 10.0f // in percentage
#define MAX_RETRIES 5 // maximum retries
#define MAX_TIMEOUT 30 // in seconds

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
        std::uniform_real_distribution<float> dist(MIN_SIGNAL_STRENGTH, MAX_SIGNAL_STRENGTH); // dBm
        return dist(gen);
    }

    float getRandomLatency() {
        std::uniform_real_distribution<float> dist(MIN_LATENCY, MAX_LATENCY); // ms
        return dist(gen);
    }

    float getRandomBandwidth() {
        std::uniform_real_distribution<float> dist(MIN_BANDWIDTH, MAX_BANDWIDTH); // Mbps
        return dist(gen);
    }

    float getRandomPacketLoss() {
        std::uniform_real_distribution<float> dist(0.0f, MAX_PACKET_LOSS); // %
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

        // Check for threshold violations
        if (params.signalStrength > MAX_SIGNAL_STRENGTH) {
            std::cout << "Warning: Signal Strength exceeds maximum threshold: " << params.signalStrength << " dBm\n";
        }
        if (params.latency > MAX_LATENCY) {
            std::cout << "Warning: Latency exceeds maximum threshold: " << params.latency << " ms\n";
        }
        if (params.bandwidth > MAX_BANDWIDTH) {
            std::cout << "Warning: Bandwidth exceeds maximum threshold: " << params.bandwidth << " Mbps\n";
        }
        if (params.packetLoss > MAX_PACKET_LOSS) {
            std::cout << "Warning: Packet Loss exceeds maximum threshold: " << params.packetLoss << " %\n";
        }

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
    ConnectionSimulator simulator(MAX_RETRIES);
    simulator.simulateConnection();
    return 0;
}