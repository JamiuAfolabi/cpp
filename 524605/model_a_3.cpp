#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>
#include <vector>

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
        if (!source.encryptedData) {
            throw std::runtime_error("Source data is NULL");
        }

        // Check if we need to reallocate
        if (source.dataSize > dataSize) {
            auto newMemory = std::make_unique<char[]>(source.dataSize);
            if (!newMemory) {
                throw std::runtime_error("Memory allocation failed during copy");
            }

            if (encryptedData) {
                std::memset(encryptedData.get(), 0, dataSize);
            }
            encryptedData = std::move(newMemory);
        }

        std::memcpy(encryptedData.get(), source.encryptedData.get(), source.dataSize);
        dataSize = source.dataSize; // Update the size
    }

    std::string getDecryptedData() const {
        if (!encryptedData) {
            throw std::runtime_error("No data available");
        }
        return encryptDecrypt(std::string(encryptedData.get(), dataSize));
    }

    ~SensitiveData() {
        if (encryptedData) {
            std::memset(encryptedData.get(), 0, dataSize);
            encryptedData.reset();
        }
    }

    // Prevent copying
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

// Class to manage an array of SensitiveData objects
class SensitiveDataArray {
private:
    std::unique_ptr<SensitiveData[]> records; // Dynamic array of SensitiveData
    size_t capacity; // Current capacity of the array
    size_t size; // Current number of records

    void resize(size_t newCapacity) {
        if (newCapacity <= capacity) {
            return; // No need to resize
        }
        
        auto newRecords = std::make_unique<SensitiveData[]>(newCapacity);
        for (size_t i = 0; i < size; ++i) {
            newRecords[i] = std::move(records[i]); // Move records to new array
        }
        records = std::move(newRecords);
        capacity = newCapacity;
    }

public:
    SensitiveDataArray(size_t initialCapacity = 2) 
      : capacity(initialCapacity), size(0) {
        records = std::make_unique<SensitiveData[]>(capacity);
    }

    void addRecord(const std::string &data) {
        if (size >= capacity) {
            resize(capacity * 2); // Double the capacity
        }
        records[size++] = SensitiveData(data); // Add the new record
    }

    void removeRecord(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        // Shift records to fill the gap
        for (size_t i = index; i < size - 1; ++i) {
            records[i] = std::move(records[i + 1]);
        }
        size--;

        // Optionally reduce capacity if size is much smaller than capacity
        if (size < capacity / 4) {
            resize(capacity / 2);
        }
    }

    void printRecords() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Record " << i + 1 << ": " << records[i].getDecryptedData() << std::endl;
        }
    }

    size_t getSize() const {
        return size;
    }

    ~SensitiveDataArray() = default; // Unique_ptr handles deallocation
};

int main() {
    try {
        SensitiveDataArray records;

        // Add some sensitive records
        records.addRecord("123-45-6789");
        records.addRecord("987-65-4321");
        records.addRecord("111-22-3333");

        // Print records
        std::cout << "Sensitive Records:" << std::endl;
        records.printRecords();

        // Remove a record
        records.removeRecord(1); // Remove second record (index 1)

        std::cout << "\nRecords after removal:" << std::endl;
        records.printRecords();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}