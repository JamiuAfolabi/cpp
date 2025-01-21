#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <vector>

const size_t INITIAL_CAPACITY = 10;
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
    std::vector<std::unique_ptr<SensitiveData>> records;
    size_t capacity;
    
    const float GROWTH_FACTOR = 1.5;

public:
    SensitiveDataArray() : capacity(INITIAL_CAPACITY) {
        records.reserve(capacity);
    }

    // Add a new record
    void addRecord(const std::string& data) {
        if (records.size() >= capacity) {
            resize(static_cast<size_t>(capacity * GROWTH_FACTOR));
        }
        records.push_back(std::make_unique<SensitiveData>(data));
    }

    // Remove a record at specific index
    void removeRecord(size_t index) {
        if (index >= records.size()) {
            throw std::out_of_range("Invalid index");
        }
        
        // Securely delete the record
        records[index].reset();
        
        // Remove the nullptr from vector
        records.erase(records.begin() + index);

        // Shrink if necessary
        if (records.size() < capacity / 2 && capacity > INITIAL_CAPACITY) {
            resize(capacity / 2);
        }
    }

    // Get decrypted data at index
    std::string getRecord(size_t index) const {
        if (index >= records.size()) {
            throw std::out_of_range("Invalid index");
        }
        return records[index]->getDecryptedData();
    }

    // Get current number of records
    size_t size() const {
        return records.size();
    }

private:
    // Resize the array
    void resize(size_t newCapacity) {
        if (newCapacity < records.size()) {
            throw std::invalid_argument("New capacity is too small");
        }
        
        records.reserve(newCapacity);
        capacity = newCapacity;
    }
};

int main() {
    try {
        // Test individual SensitiveData object
        std::cout << "Testing individual SensitiveData object:" << std::endl;
        SensitiveData ssn1("123-45-6789");
        SensitiveData ssn2("987-65-4321");
        ssn2.secureDataCopy(ssn1);
        std::cout << "Decrypted SSN1: " << ssn1.getDecryptedData() << std::endl;
        std::cout << "Decrypted SSN2 (after copy): " << ssn2.getDecryptedData() << std::endl;

        // Test SensitiveDataArray
        std::cout << "\nTesting SensitiveDataArray:" << std::endl;
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