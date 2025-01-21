#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <list>

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

    // Get size of data
    size_t getSize() const {
        return dataSize;
    }

    // Check if data is valid
    bool isValid() const {
        return encryptedData != nullptr && dataSize > 0;
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

class SensitiveDataArray {
private:
    std::list<std::unique_ptr<SensitiveData>> records;

public:
    // Add a new record
    void addRecord(const std::string& data) {
        records.emplace_back(std::make_unique<SensitiveData>(data));
    }

    // Remove a record at specific index
    void removeRecord(size_t index) {
        if (index >= records.size()) {
            throw std::out_of_range("Invalid index");
        }

        // Securely delete the record by erasing the iterator
        records.erase(std::next(records.begin(), index));
    }

    // Get decrypted data at index
    std::string getRecord(size_t index) const {
        if (index >= records.size()) {
            throw std::out_of_range("Invalid index");
        }

        auto& record = records.at(index);
        if (!record) {
            throw std::runtime_error("Record is empty");
        }
        return record->getDecryptedData();
    }

    // Get current number of records
    size_t size() const {
        return records.size();
    }
};

int main() {
    try {
        SensitiveDataArray governmentRecords;

        // Add multiple records
        governmentRecords.addRecord("123-45-6789");
        governmentRecords.addRecord("987-65-4321");
        governmentRecords.addRecord("456-78-9012");

        // Access and print records
        std::cout << "Total records: " << governmentRecords.size() << std::endl;
        for (size_t i = 0; i < governmentRecords.size(); ++i) {
            std::cout << "Record " << i << ": " << governmentRecords.getRecord(i) << std::endl;
        }

        // Remove a record
        governmentRecords.removeRecord(1);
        std::cout << "\nAfter removing record 1:" << std::endl;
        std::cout << "Total records: " << governmentRecords.size() << std::endl;
        
        // Print remaining records
        for (size_t i = 0; i < governmentRecords.size(); ++i) {
            std::cout << "Record " << i << ": " << governmentRecords.getRecord(i) << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}