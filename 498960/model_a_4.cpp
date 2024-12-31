#include <iostream>
#include <string>
#include <vector>

class FNV1aHash {
public:
    static const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
    static const uint64_t FNVPrime = 1099511628211ULL;

    static uint64_t hash(const std::string& input) {
        uint64_t hashValue = FNV_OFFSET_BASIS;
        for (char c : input) {
            hashValue *= FNVPrime;
            hashValue ^= static_cast<uint64_t>(c);
        }
        return hashValue;
    }
};

class HMAC {
public:
    static uint64_t computeHMAC(const std::string& key, const std::string& message) {
        const size_t blockSize = 64; // FNV-1a block size
        std::string modifiedKey = key;

        // Step 1: Key preparation
        if (key.size() > blockSize) {
            modifiedKey = std::to_string(FNV1aHash::hash(key));
        } else if (key.size() < blockSize) {
            modifiedKey.append(blockSize - key.size(), '\0');
        }

        // Inner and outer padding
        std::string innerPadding(blockSize, 0x36);
        std::string outerPadding(blockSize, 0x5C);

        for (size_t i = 0; i < modifiedKey.size(); ++i) {
            innerPadding[i] ^= modifiedKey[i];
            outerPadding[i] ^= modifiedKey[i];
        }

        // Step 2: Inner hash
        uint64_t innerHash = FNV1aHash::hash(innerPadding + message);
        
        // Step 3: Outer hash
        uint64_t hmacValue = FNV1aHash::hash(outerPadding + std::to_string(innerHash));

        return hmacValue;
    }
};

int main() {
    std::string key = "secret_key";
    std::string message = "message";

    uint64_t hmacValue = HMAC::computeHMAC(key, message);
    std::cout << "HMAC using FNV-1a: " << hmacValue << std::endl;

    return 0;
}