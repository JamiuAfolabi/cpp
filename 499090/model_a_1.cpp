#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>

// Simulated NFC reader class
class NFCReader {
public:
    // Simulate reading NFC data
    std::string readNFC() {
        // Generate mock NFC data
        std::string nfcData = generateRandomNFCData();
        std::cout << "NFC Data Read: " << nfcData << std::endl;
        return nfcData;
    }

private:
    // Function to generate random NFC data (simulate different NFC tags)
    std::string generateRandomNFCData() {
        const std::string characters = "0123456789ABCDEF";
        std::string result;
        std::uniform_int_distribution<size_t> dist(0, characters.size() - 1);
        std::default_random_engine generator(std::random_device{}());

        // Simulate a hex encoded NFC tag
        for (size_t i = 0; i < 8; ++i) {
            result += characters[dist(generator)];
        }
        return result;
    }
};

// Main program
int main() {
    NFCReader nfcReader;

    // Simulate NFC reading every second
    std::cout << "Starting NFC Reader Simulation...\n";

    for (int i = 0; i < 10; ++i) { // Simulate reading NFC data 10 times for 10 seconds
        nfcReader.readNFC();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay between reads
    }

    std::cout << "NFC Reader Simulation Complete.\n";
    return 0;
}