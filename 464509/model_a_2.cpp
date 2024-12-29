#include <iostream>
#include <vector>
#include <stdexcept>

const int BITS_PER_INT = 8;  // Number of bits per integer (e.g., 8 for values 0-255)
const int TOTAL_INTS = 4;     // Total number of integers to serialize
const unsigned int MASK = (1 << BITS_PER_INT) - 1; // Mask for extracting BITS_PER_INT bits (0xFF for 8 bits)

// Function to serialize an array of integers
unsigned int serialize(const std::vector<int>& values) {
    if (values.size() > TOTAL_INTS) {
        throw std::invalid_argument("Input array exceeds the maximum number of integers.");
    }

    unsigned int result = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] < 0 || values[i] > MASK) {
            throw std::invalid_argument("Integer values must be in the range 0 to " + std::to_string(MASK) + ".");
        }
        
        // Apply the mask to ensure only the relevant bits are considered
        result |= (static_cast<unsigned int>(values[i]) & MASK) << (i * BITS_PER_INT);
    }

    return result;
}

// Function to deserialize an integer back into an array of integers
std::vector<int> deserialize(unsigned int value) {
    std::vector<int> result;
    for (int i = 0; i < TOTAL_INTS; ++i) {
        // Extract only the last BITS_PER_INT bits
        int extractedValue = (value >> (i * BITS_PER_INT)) & MASK;
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