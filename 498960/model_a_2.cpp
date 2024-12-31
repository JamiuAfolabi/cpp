#include <iostream>
#include <string>

class FNV1aHash {
public:
    // FNV-1a hash parameters
    static const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
    static const uint64_t FNVPrime = 1099511628211ULL;

    // Hash function taking a string input
    static uint64_t hash(const std::string& input) {
        uint64_t hashValue = FNV_OFFSET_BASIS;

        for (char c : input) {
            hashValue *= FNVPrime;                // Multiply by the prime first
            hashValue ^= static_cast<uint64_t>(c); // Then XOR with the byte
        }

        return hashValue;
    }
};

int main() {
    std::string data;
    std::cout << "Enter data to hash: ";
    std::getline(std::cin, data);

    uint64_t hashValue = FNV1aHash::hash(data);
    std::cout << "FNV-1a Hash: " << hashValue << std::endl;

    return 0;
}