#include <iostream>
#include <stdexcept>

template<typename K, typename V>
class RingBufferCache {
public:
    RingBufferCache(size_t maxSize) : maxSize(maxSize), head(0), size(0) {
        keys = new K[maxSize];
        values = new V[maxSize];
    }

    ~RingBufferCache() {
        delete[] keys;
        delete[] values;
    }

    void put(const K& key, const V& value) {
        if (size < maxSize) {
            // Cache is not full
            keys[size] = key;
            values[size] = value;
            size++;
        } else {
            // Cache is full, overwrite the oldest entry
            keys[head] = key;
            values[head] = value;
        }
        // Move head forward in a circular manner
        head = (head + 1) % maxSize;
    }

    bool get(const K& key, V& value) {
        for (size_t i = 0; i < size; ++i) {
            size_t index = (head + maxSize - size + i) % maxSize; // Calculate the index in a circular manner
            if (keys[index] == key) {
                value = values[index];
                return true;
            }
        }
        return false; // Key not found
    }

    void printCache() const {
        for (size_t i = 0; i < size; ++i) {
            size_t index = (head + maxSize - size + i) % maxSize; // Calculate the index in a circular manner
            std::cout << keys[index] << ": " << values[index] << std::endl;
        }
    }

private:
    size_t maxSize;
    size_t head; // Points to the next position to overwrite
    size_t size; // Current number of entries
    K* keys;     // Array to hold keys
    V* values;   // Array to hold values
};

// Example usage
int main() {
    RingBufferCache<int, std::string> cache(10);

    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");

    cache.printCache();

    // Retrieve a value
    std::string value;
    if (cache.get(2, value)) {
        std::cout << "Found key 2: " << value << std::endl;
    }

    // Add more values to exceed capacity
    for (int i = 4; i <= 12; ++i) {
        cache.put(i, "Value " + std::to_string(i));
    }

    std::cout << "Cache after exceeding capacity:" << std::endl;
    cache.printCache();

    return 0;
}