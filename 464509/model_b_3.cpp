#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint> // For portable integer types (uint16_t)

const int BITS_PER_INT = 16; // Now set to 16 for 16-bit integers
const int TOTAL_INTS = 3;    // Adjust the number of integers you can serialize (it will be limited now)

const uint16_t MASK = (1 << BITS_PER_INT) - 1; // 0xFFFF for 16 bits

// Function to serialize an array of integers (modified to support 16-bit integers)
uint64_t serialize(const std::vector<uint16_t>& values) {
    if (values.size() > TOTAL_INTS) {
        throw std::invalid_argument("Input array exceeds the maximum number of integers.");
    }

    uint64_t result = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] > MASK) {
            throw std::invalid_argument("Integer values must be in the range 0 to " + std::to_string(MASK) + ".");
        }
        // Apply the mask
        result |= static_cast<uint64_t>(values[i] & MASK) << (i * BITS_PER_INT);
    }

    return result;
}

// Function to deserialize an integer back into an array of integers (modified to support 16-bit integers)
std::vector<uint16_t> deserialize(uint64_t value) {
    std::vector<uint16_t> result;
    for (int i = 0; i < TOTAL_INTS; ++i) {
        // Extract 16 bits and apply the mask
        uint16_t extractedValue = (value >> (i * BITS_PER_INT)) & MASK;
        result.push_back(extractedValue);
    }
    return result;
}

int main() {
    // Example values with 16 bits: can range from 0 to 0xFFFF
    std::vector<uint16_t> values {1234, 5678, 9012};

    try {
        // Serialize the array of integers
        uint64_t serializedValue = serialize(values);
        std::cout << "Serialized value: " << std::hex << serializedValue << std::endl;

        // Deserialize back to an array
        std::vector<uint16_t> deserializedValues = deserialize(serializedValue);
        std::cout << "Deserialized values: ";
        for (uint16_t value : deserializedValues) {
            std::cout << std::hex << value << " ";
        }
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}