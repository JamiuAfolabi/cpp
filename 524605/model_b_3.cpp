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
        output[i] = data[i] ^ key;
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

class SensitiveDataArray {
private:
    std::unique_ptr<SensitiveData[]> records;
    size_t currentSize;
    size_t capacity;

public:
    // Default constructor (start with empty array)
    SensitiveDataArray()
        : currentSize(0), capacity(0) {
        allocateRecordMemory(1); // Start with a small initial capacity
    }

    // Destructor to free all allocated memory
    ~SensitiveDataArray() {
        clear();
    }

    // Copy constructor and copy assignment operator (deleted due to sensitive data)
    SensitiveDataArray(const SensitiveDataArray &) = delete;
    SensitiveDataArray &operator=(const SensitiveDataArray &) = delete;

    // Move constructor and move assignment operator
    SensitiveDataArray(SensitiveDataArray &&other) noexcept {
        currentSize = other.currentSize;
        capacity = other.capacity;
        records = std::move(other.records);
        other.records = nullptr;
        other.currentSize = 0;
        other.capacity = 0;
    }

    SensitiveDataArray &operator=(SensitiveDataArray &&other) noexcept {
        if (this != &other) {
            clear();
            currentSize = other.currentSize;
            capacity = other.capacity;
            records = std::move(other.records);
            other.records = nullptr;
            other.currentSize = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Add a new sensitive data record
    void addRecord(const std::string &data) {
        if (currentSize == capacity) {
            allocateRecordMemory(currentSize * 2); // Double capacity when needed
        }

        records[currentSize] = SensitiveData(data);
        currentSize++;
    }

    // Remove the record at the specified index
    void removeRecord(size_t index) {
        if (index < currentSize) {
            records[index].~SensitiveData();  // Explicit destructor call to securely clear memory
            for (size_t i = index + 1; i < currentSize; ++i) {
                records[i - 1] = std::move(records[i]);
            }
            currentSize--;
        }
    }

    // Accessor to get a record at the specified index
    SensitiveData& getRecord(size_t index) {
        if (index < currentSize) {
            return records[index];
        } else {
            throw std::out_of_range("Record index out of bounds.");
        }
    }

    // Resize the array to fit the new size
    void resize(size_t newSize) {
        if (newSize == currentSize) {
            return; // No need to do anything if sizes are the same
        }

        if (newSize == 0) {
            clear();
            return;
        }

        // Resize capacity if necessary
        if (newSize > capacity) {
            allocateRecordMemory(newSize);
        }

        size_t newCurrentSize = std::min(newSize, currentSize);

        // Move elements if resizing smaller than current size
        while (currentSize > newCurrentSize) {
            records[currentSize - 1].~SensitiveData();
            currentSize--;
        }

        // Set the correct current size
        currentSize = newCurrentSize;
    }

    // Clear the entire array
    void clear() {
        for (size_t i = 0; i < currentSize; ++i) {
            records[i].~SensitiveData();
        }
        currentSize = 0;
    }

    size_t size() const {
        return currentSize;
    }

private:
    // Reallocate memory for records with a new capacity
    void allocateRecordMemory(size_t newCapacity) {
        auto newMemory = std::make_unique<SensitiveData[]>(newCapacity);
        if (!newMemory) {
            throw std::runtime_error("Memory allocation failed for records.");
        }

        // Copy existing records (if any)
        if (currentSize > 0) {
            for (size_t i = 0; i < currentSize; ++i) {
                newMemory[i] = std::move(records[i]);
            }
        }

        // Transfer ownership
        records = std::move(newMemory);
        capacity = newCapacity;
    }
};

int main() {
    try {
        SensitiveDataArray records;

        records.addRecord("123-45-6789");
        records.addRecord("987-65-4321");
        records.addRecord("456-78-9012");

        std::cout << "Record 1: " << records.getRecord(0).getDecryptedData() << std::endl;

        records.removeRecord(1);

        std::cout << "\nAfter removing record 1:" << std::endl;
        for (size_t i = 0; i < records.size(); ++i) {
            std::cout << "Record " << (i + 1) << ": " << records.getRecord(i).getDecryptedData() << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}