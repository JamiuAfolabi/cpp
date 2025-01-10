#include <iostream>
#include <random>
#include <thread>
#include <chrono>

#define MAX_SIGNAL_STRENGTH -30.0f
#define MIN_SIGNAL_STRENGTH -100.0f
#define MAX_LATENCY 100.0f
#define MAX_BANDWIDTH 100.0f
#define MAX_PACKET_LOSS 10.0f
#define MAX_TIMEOUT_SECONDS 30
#define MAX_RETRIES 5

class ConnectionParameters {
public:
    float signalStrength;
    bool connectionStatus;
    float latency;
    float bandwidth;
    float packetLoss;
    bool connectionTimeout;

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
        std::uniform_real_distribution<float> dist(MIN_SIGNAL_STRENGTH, MAX_SIGNAL_STRENGTH);
        float result = dist(gen);
        if (result < MIN_SIGNAL_STRENGTH || result > MAX_SIGNAL_STRENGTH) {
            std::cout << "Generated signal strength exceeds threshold. Setting to minimum.\n";
            result = MIN_SIGNAL_STRENGTH;
        }
        return result;
    }

    float getRandomLatency() {
        std::uniform_real_distribution<float> dist(0.0f, MAX_LATENCY);
        float result = dist(gen);
        if (result > MAX_LATENCY) {
            std::cout << "Generated latency exceeds threshold. Setting to maximum.\n";
            result = MAX_LATENCY;
        }
        return result;
    }

    float getRandomBandwidth() {
        std::uniform_real_distribution<float> dist(0.0f, MAX_BANDWIDTH);
        float result = dist(gen);
        if (result > MAX_BANDWIDTH) {
            std::cout << "Generated bandwidth exceeds threshold. Setting to maximum.\n";
            result = MAX_BANDWIDTH;
        }
        return result;
    }

    float getRandomPacketLoss() {
        std::uniform_real_distribution<float> dist(0.0f, MAX_PACKET_LOSS);
        float result = dist(gen);
        if (result > MAX_PACKET_LOSS) {
            std::cout << "Generated packet loss exceeds threshold. Setting to maximum.\n";
            result = MAX_PACKET_LOSS;
        }
        return result;
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
        params.connectionStatus = !params.connectionTimeout;
        return params;
    }
};

class ConnectionSimulator {
private:
    NetworkTower tower;

public:
    ConnectionSimulator() {}

    void simulateConnection() {
        for (int attempt = 1; attempt <= MAX_RETRIES; ++attempt) {
            std::cout << "Attempt " << attempt << ":\n";
            ConnectionParameters params = tower.connect();
            params.display();
            std::cout << "-------------------------------------\n";

            if (!params.connectionStatus) {
                std::cout << "Retrying...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            } else {
                break;
            }
        }
    }
};

int main() {
    ConnectionSimulator simulator;
    simulator.simulateConnection();
    return 0;
}