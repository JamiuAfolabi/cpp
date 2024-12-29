#include <iostream>
#include <vector>
#include <stdexcept>

unsigned int createMask(int bits) {
    if (bits <= 0 || bits > 32) {
        throw std::invalid_argument("BIT size must be between 1 and 32.");
    }
    return (1u << bits) - 1; // Generate mask for the given number of bits
}

// Function to serialize an array of integers
unsigned int serialize(const std::vector<int>& values, int bitsPerInt) {
    unsigned int mask = createMask(bitsPerInt);
    int totalInts = values.size();

    if (totalInts * bitsPerInt > 32) { // Check if the total exceeds 32 bits
        throw std::invalid_argument("Total bits exceed 32 bits limit.");
    }

    unsigned int result = 0;
    for (size_t i = 0; i < totalInts; ++i) {
        if (values[i] < 0 || (values[i] & mask) != values[i]) { // Ensure value fits within mask
            throw std::invalid_argument("Integer values must fit within the specified range.");
        }
        
        result |= (static_cast<unsigned int>(values[i]) & mask) << (i * bitsPerInt);
    }

    return result;
}

// Function to deserialize an integer back into an array of integers
std::vector<int> deserialize(unsigned int value, int bitsPerInt, int totalInts) {
    unsigned int mask = createMask(bitsPerInt);
    std::vector<int> result;

    for (int i = 0; i < totalInts; ++i) {
        int extractedValue = (value >> (i * bitsPerInt)) & mask;
        result.push_back(extractedValue);
    }
    return result;
}

int main() {
    std::vector<int> values {255, 132, 64, 128}; // Example values with 8 bits
    int bitsPerInt = 8; // Change this value to 16, 12, etc., as needed

    try {
        // Serialize the array of integers
        unsigned int serializedValue = serialize(values, bitsPerInt);
        std::cout << "Serialized value: " << serializedValue << std::endl;

        // Deserialize back to an array
        std::vector<int> deserializedValues = deserialize(serializedValue, bitsPerInt, values.size());
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