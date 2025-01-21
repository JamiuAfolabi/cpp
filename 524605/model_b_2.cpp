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
    size_t dataSize; // Store the size of the data

public:
    SensitiveData(const std::string &data) {
        if (data.empty()) {
            throw std::invalid_argument("Data cannot be empty");
        }
        dataSize = data.size();
        encryptedData = std::make_unique<char[]>(dataSize);
        std::string encrypted = encryptDecrypt(data);
        std::memcpy(encryptedData.get(), encrypted.c_str(), dataSize);
    }

    void secureDataCopy(const SensitiveData &source) {
        if (this == &source) {
            return; // Self-copy is empty
        }

        if (source.dataSize == 0) {
            dataSize = 0;
            encryptedData.reset();
            return; // Source is empty
        }

        if (!encryptedData) {
            // This case is handled by the default allocator in std::unique_ptr
        }
 
        if (source.dataSize != dataSize) {
            char *newData = new char[source.dataSize];  // Temporary raw data storage
            if (!newData) {
                throw std::runtime_error("Memory allocation failed");
            }

            // Copy encrypted data
            std::string sourceEncrypted = std::string(source.encryptedData.get(), source.dataSize);
            std::string encrypted = encryptDecrypt(sourceEncrypted);
            std::memcpy(newData, encrypted.c_str(), source.dataSize);

            // Old data destruction
            std::memset(encryptedData.get(), 0, dataSize);
            // Potentially destructive replace (bad result indicates larger realloc problem)
            if (encryptedData.swap(std::make_unique<char[]>(source.dataSize)) == 0) {
                std::strcpy(encryptedData.get(), newData);
            } 
            else {
                throw std::runtime_error("Memory replacement failed during reallocation");
            }

            delete[] newData;
        } else {
            // Source and destination are same size
            std::string sourceEncrypted = std::string(source.encryptedData.get(), source.dataSize);
            std::string encrypted = encryptDecrypt(sourceEncrypted);
            std::memcpy(encryptedData.get(), encrypted.c_str(), dataSize);
        }
        dataSize = source.dataSize;
    }

    // Decrypt data for reading
    std::string getDecryptedData() const {
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