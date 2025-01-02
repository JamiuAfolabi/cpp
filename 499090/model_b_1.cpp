#include <iostream>
#include <cstdlib>   // For rand() and RAND_MAX
#include <ctime>     // For seeding rand()
#include <string>
#include <thread>
#include <chrono>

const int MAX_NFC_DATA_LENGTH = 16;

class NFCDigitalTwin {
private:
    std::string nfcData;

public:
    void simulateNFCRead() {
        // Seed the random number generator
        srand(static_cast<unsigned>(std::time(0)));

        // Simulate NFC data read loop
        while (true) {
            int dataLength = rand() % (MAX_NFC_DATA_LENGTH + 1);
            nfcData = "";
            for (int i = 0; i < dataLength; i++) {
                char randomChar = static_cast<char>(rand() % 128 + 32);  // Generate random ASCII character
                nfcData += randomChar;
            }

            std::cout << "NFC Data Read: " << nfcData << std::endl;

            // Simulate a delay between reads
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    std::string getNFCData() const {
        return nfcData;
    }
};

int main() {
    NFCDigitalTwin nfcTwin;

    std::thread nfcReadThread(&NFCDigitalTwin::simulateNFCRead, &nfcTwin);

    // Main application thread, can perform other tasks here
    while (true) {
        std::cout << "Current NFC Data: " << nfcTwin.getNFCData() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // To exit the program, you can press Ctrl+C in the console
    // Properly join the thread to clean up resources
    nfcReadThread.join();
    return 0;
}