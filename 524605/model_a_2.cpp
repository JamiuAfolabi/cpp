#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

// Simple XOR encryption for demonstration purposes
std::string encryptDecrypt(const std::string &data) {
    const char key = 'K'; // Simple example key
    std::string output = data;
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] = data[i] ^ key; // XOR each character
    }
    return output;
}

class SensitiveData {
private:
    std::unique_ptr<char[]> encryptedData; // Use char array for raw data
    size_t dataSize;

public:
    // Constructor to initialize with data
    SensitiveData(const std::string &data) {
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty");
        }
        dataSize = data.size();
        encryptedData = std::make_unique<char[]>(dataSize);
        std::string encrypted = encryptDecrypt(data);
        std::memcpy(encryptedData.get(), encrypted.c_str(), dataSize);
    }

    // Securely copy encrypted data between SensitiveData objects
    void secureDataCopy(const SensitiveData &source) {
        if (!source.encryptedData) {
            throw std::runtime_error("Source data is NULL");
        }

        // Check if we need to reallocate
        if (source.dataSize > dataSize) {
            // Allocate new memory
            auto newMemory = std::make_unique<char[]>(source.dataSize);
            if (!newMemory) {
                throw std::runtime_error("Memory allocation failed during copy");
            }

            // Securely delete old memory
            if (encryptedData) {
                std::memset(encryptedData.get(), 0, dataSize);
            }

            // Transfer ownership
            encryptedData = std::move(newMemory);
        }

        // Copy the source encrypted data
        std::memcpy(encryptedData.get(), source.encryptedData.get(), source.dataSize);
        dataSize = source.dataSize; // Update the size
    }

    // Decrypt data for reading
    std::string getDecryptedData() const {
        if (!encryptedData) {
            throw std::runtime_error("No data available");
        }
        std::string decrypted = encryptDecrypt(std::string(encryptedData.get(), dataSize));
        return decrypted;
    }

    // Override destructor to securely clear memory
    ~SensitiveData() {
        if (encryptedData) {
            // Clear sensitive data
            std::memset(encryptedData.get(), 0, dataSize);
            encryptedData.reset(); // Memory will be freed automatically
        }
    }

    // Prevent copying (rule of three)
    SensitiveData(const SensitiveData &) = delete;
    SensitiveData &operator=(const SensitiveData &) = delete;

    // Allow move semantics
    SensitiveData(SensitiveData &&other) noexcept
        : encryptedData(std::move(other.encryptedData)), dataSize(other.dataSize) {
        other.dataSize = 0; // Prevent double deletion
    }

    SensitiveData &operator=(SensitiveData &&other) noexcept {
        if (this != &other) {
            if (encryptedData) {
                std::memset(encryptedData.get(), 0, dataSize);
            }
            encryptedData = std::move(other.encryptedData);
            dataSize = other.dataSize;
            other.dataSize = 0; // Prevent double deletion
        }
        return *this;
    }
};

int main() {
    try {
        // Store sensitive data (example: a social security number)
        SensitiveData ssn1("123-45-6789");
        SensitiveData ssn2("987-65-4321");

        // Securely copy the encrypted data from ssn1 to ssn2
        ssn2.secureDataCopy(ssn1);

        // Access decrypted data
        std::cout << "Decrypted SSN1: " << ssn1.getDecryptedData() << std::endl;
        std::cout << "Decrypted SSN2 (after copy): " << ssn2.getDecryptedData() << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}