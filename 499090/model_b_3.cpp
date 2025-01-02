#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>

class NFCReader {
public:
    std::string readNFC() {
        std::string nfcData = generateRandomNFCData();
        std::cout << "NFC Data Read: " << nfcData << std::endl;
        return nfcData;
    }

private:
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
};

// Extended Network connection states
enum class NetworkState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR,
    RETRY
};

// Function to simulate network connection handling
void handleNetworkState(NetworkState state) {
    switch (state) {
        case NetworkState::DISCONNECTED:
            std::cout << "Network Status: DISCONNECTED." << std::endl;
            break;
        case NetworkState::CONNECTING:
            std::cout << "Network Status: CONNECTING..." << std::endl;
            break;
        case NetworkState::CONNECTED:
            std::cout << "Network Status: CONNECTED." << std::endl;
            break;
        case NetworkState::ERROR:
            std::cout << "Network Status: ERROR. Retrying connection..." << std::endl;
            break;
        case NetworkState::RETRY:
            std::cout << "Network Status: Retrying connection..." << std::endl;
            break;
        default:
            std::cout << "Unknown Network Status." << std::endl;
            break;
    }
}

// Function to transition to a new state with probability of error
NetworkState transitionToNewState(NetworkState currentState) {
    switch (currentState) {
        case NetworkState::DISCONNECTED:
            return NetworkState::CONNECTING;
        case NetworkState::CONNECTING:
            // 20% chance of error during connection
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 5);
            int randomNum = dis(gen);
            if (randomNum == 1) {
                return NetworkState::ERROR;
            }
            return NetworkState::CONNECTED;
        case NetworkState::CONNECTED:
            // 10% chance of error while connected
            randomNum = dis(gen);
            if (randomNum == 1) {
                return NetworkState::ERROR;
            }
            return NetworkState::CONNECTED;
        case NetworkState::ERROR:
            return NetworkState::RETRY;
        case NetworkState::RETRY:
            // 30% chance of success on retry, otherwise go back to disconnected
            randomNum = dis(gen);
            if (randomNum <= 3) {
                return NetworkState::DISCONNECTED;
            }
            return NetworkState::CONNECTING;
        default:
            return currentState;
    }
}

// Main program
int main() {
    NFCReader nfcReader;
    NetworkState currentState = NetworkState::DISCONNECTED;

    std::cout << "Starting Network Simulation..." << std::endl;

    while (true) {
        handleNetworkState(currentState);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        currentState = transitionToNewState(currentState);
        if (currentState == NetworkState::CONNECTED) {
            std::cout << "Starting NFC Reader Simulation...\n";
            for (int i = 0; i < 10; ++i) {
                nfcReader.readNFC();
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay between reads
            }
            std::cout << "NFC Reader Simulation Complete.\n";
        }
        if (currentState == NetworkState::DISCONNECTED) {
            break;
        }
    }

    std::cout << "Network Simulation Complete.\n";
    return 0;
}