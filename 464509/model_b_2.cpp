#include <iostream>
#include <vector>
#include <stdexcept>

const int BITS_PER_INT = 8;  // Number of bits per integer
const int TOTAL_INTS = 4;     // Total number of integers to serialize

unsigned int serialize(const std::vector<int>& values) {
    if (values.size() > TOTAL_INTS) {
        throw std::invalid_argument("Input array exceeds the maximum number of integers.");
    }

    unsigned int result = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        int value = values[i];
        if (value < 0 || value > (1 << BITS_PER_INT) - 1) {
            throw std::invalid_argument("Integer values must be within the range [0, " + std::to_string((1 << BITS_PER_INT) - 1) + "].");
        }
        // Apply the mask to ensure only the valid bits are used
        result |= (static_cast<unsigned int>(value) & ((1 << BITS_PER_INT) - 1)) << (i * BITS_PER_INT);
    }

    return result;
}

// Function to deserialize an integer back into an array of integers
std::vector<int> deserialize(unsigned int value) {
    std::vector<int> result;
    for (int i = 0; i < TOTAL_INTS; ++i) {
        int extractedValue = (value >> (i * BITS_PER_INT)) & 0xFF;  // Extract 8 bits
        result.push_back(extractedValue);
    }
    return result;
}

int main() {
    // Example values to serialize
    std::vector<int> values {12, 34, 56, 78};

    try {
        // Serialize the array of integers
        unsigned int serializedValue = serialize(values);
        std::cout << "Serialized value: " << serializedValue << std::endl;

        // Deserialize back to an array
        std::vector<int> deserializedValues = deserialize(serializedValue);
        std::cout << "Deserialized values: ";
        for (int value : deserializedValues) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}