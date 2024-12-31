#include <iostream>
#include <string>

class FNV1aHash {
public:
    // FNV-1a hash parameters
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

// HMAC-FNV1a implementation
class HMAC_FNV1a {
public:
    static uint64_t generate(const std::string& key, const std::string& message) {
        uint64_t opad = 0x5C5C5C5C5C5C5C5CULL; // Outer padding as uint64_t for 64-bit FNV-1a
        uint64_t ipad = 0x3636363636363636ULL; // Inner padding

        // Step 1: Pad the key
        std::string paddedKey(64, 0x00);
        for (size_t i = 0; i < std::min(key.size(), paddedKey.size()); i++) {
            paddedKey[i] = static_cast<uint8_t>(key[i]);
        }

        // Step 2: Calculate inner hash
        std::string o_padded_key = template_default_value(paddedKey, opad);
        std::string message_ipad = template_default_value(message, ipad);
        std::string innerData = o_padded_key + message_ipad;
        uint64_t innerHash = FNV1aHash::hash(innerData);

        // Step 3: Calculate outer hash
        std::string message_opad = template_default_value(message, opad);
        std::string outerData = o_padded_key + template_int_to_bytes(innerHash);
        uint64_t outerHash = FNV1aHash::hash(outerData);

        return outerHash;
    }
private:
    static std::string template_default_value(const std::string & input, uint64_t defaultValue) {
	    std::string res(64, (uint8_t)defaultValue);
	    std::copy(input.begin(), input.end(), res.begin());
	    return res;
    }
    //helper function to convert int to byte 
    static std::string template_int_to_bytes(uint64_t value) {
        std::string bytes;
        for (int i = 7; i >= 0; i--) {
            bytes.push_back((uint8_t) (value >> (8 * i) & 0xFF));
        }
        return bytes;
    }
};

int main() {
    std::string key = "YourSecretKey";
    std::string message = "The message to be authenticated";

    uint64_t hmacValue = HMAC_FNV1a::generate(key, message);
    std::cout << "HMAC-FNV1a Hash: " << hmacValue << std::endl;