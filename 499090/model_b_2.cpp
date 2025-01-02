#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <algorithm>

enum class NetworkState {
    Connected,
    Disconnected,
    Connecting
};

// Simulated NFC reader class
class NFCReader {
public:
    NFCReader(NetworkState initialState = NetworkState::Disconnected) : networkState(initialState) {}

    // Simulate reading NFC data. Depends on the current network state
    std::string readNFC() {
        std::string nfcData;
        switch (networkState) {
            case NetworkState::Connected:
                nfcData = generateRandomNFCData();
                std::cout << "NFC Data Read: " << nfcData << std::endl;
                break;
            case NetworkState::Disconnected:
                nfcData = "NFC Reading Disabled - Network Disconnected";
                std::cout << nfcData << std::endl;
                break;
            case NetworkState::Connecting:
                nfcData = "NFC Reading Paused - Connecting...";
                std::cout << nfcData << std::endl;
                break;
        }
        return nfcData;
    }

    // Transition to a new network state
    void transitionToState(NetworkState newState) {
        networkState = newState;
    }

private:
    // Function to generate random NFC data
    std::string generateRandomNFCData() {
        const std::string characters = "0123456789ABCDEF";
        std::string result;
        std::uniform_int_distribution<size_t> dist(0, characters.size() - 1);
        std::default_random_engine generator(std::random_device{}());

        for (size_t i = 0; i < 8; ++i) {
            result += characters[dist(generator)];
        }
        return result;
    }

    NetworkState networkState;
};

// Main program
int main() {
    NFCReader nfcReader;

    std::cout << "Starting NFC Reader Simulation...\n";

    // State transition timer (change state every 3 seconds)
    const int stateChangeInterval = 3;

    for (int i = 0; i < 10; ++i) {
        nfcReader.readNFC();

        // Simulate random network state changes
        int randomState = std::rand() % 3;
        NetworkState newState;
        switch (randomState) {
            case 0:
                newState = NetworkState::Connected;
                break;
            case 1:
                newState = NetworkState::Disconnected;
                break;
            default:
                newState = NetworkState::Connecting;
                break;
        }

        nfcReader.transitionToState(newState);
        std::cout << "Network Connection State: " << [](NetworkState state) {
            switch (state) {
                case NetworkState::Connected:
                    return "Connected";
                case NetworkState::Disconnected:
                    return "Disconnected";
                case NetworkState::Connecting:
                    return "Connecting";
            }
        }(nfcReader.networkState) << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "NFC Reader Simulation Complete.\n";
    return 0;
}