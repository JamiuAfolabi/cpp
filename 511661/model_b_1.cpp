
#include <iostream>
#include <ctime>
#include <cstdlib>  // For rand() function
#include <vector>

#define MAX_RETRIES 3

struct NetworkParameters {
    int signalStrength;        // 0-100
    bool connectionStatus;   // true: connected, false: disconnected
    float latency;             // In milliseconds
    double bandwidth;         // In Mbps
    float packetLoss;         // In percentage
    int connectionTimeout;    // In seconds
};

NetworkParameters simulateNetworkConnection() {
    NetworkParameters params;
    // Simulate random network parameters
    params.signalStrength = std::rand() % 101;
    params.connectionStatus = (std::rand() % 2) == 0;
    params.latency = 50.0f + (std::rand() % 200) / 10.0f;
    params.bandwidth = 5.0 + (std::rand() % 50) / 10.0;
    params.packetLoss = (std::rand() % 10) / 10.0f;
    params.connectionTimeout = std::rand() % 5 + 2;

    return params;
}

void connectToNetwork(int& retryCount) {
    retryCount = 0;
    bool connected = false;

    while (!connected && retryCount < MAX_RETRIES) {
        retryCount++;
        std::cout << "Retry Attempt " << retryCount << "..." << std::endl;

        NetworkParameters params = simulateNetworkConnection();
        std::cout << "Signal Strength: " << params.signalStrength << "%" << std::endl;
        std::cout << "Connection Status: " << (params.connectionStatus ? "Connected" : "Disconnected") << std::endl;
        std::cout << "Latency: " << params.latency << " ms" << std::endl;
        std::cout << "Bandwidth: " << params.bandwidth << " Mbps" << std::endl;
        std::cout << "Packet Loss: " << params.packetLoss << "%" << std::endl;
        std::cout << "Connection Timeout: " << params.connectionTimeout << " seconds" << std::endl;

        connected = params.connectionStatus;
    }

    if (connected) {
        std::cout << "Connected to the network." << std::endl;
    } else {
        std::cout << "Failed to connect to the network after " << retryCount << " retries." << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int retryCount = 0;
    std::cout << "Simulating Network Connection..." << std::endl;
    connectToNetwork(retryCount);

    return 0;
}
