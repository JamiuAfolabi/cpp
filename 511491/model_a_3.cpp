#include <iostream>
#include <unordered_map>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <thread>

template<typename K, typename V>
class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize) {}

    void put(const K& key, const V& value) {
        std::unique_lock<std::mutex> lock(mutex_); // Lock for writing
        if (cacheMap.find(key) != cacheMap.end()) {
            // Update the value of the existing entry in the deque
            cacheMap[key]->second = value;
            return;
        }

        if (cacheList.size() == maxSize) {
            // Cache is full, remove the oldest entry
            K oldestKey = cacheList.front().first;
            cacheList.pop_front();
            cacheMap.erase(oldestKey);
        }

        // Insert the new entry at the back of the list
        cacheList.push_back(std::make_pair(key, value));
        cacheMap[key] = --cacheList.end(); // The iterator to the new entry
    }

    bool get(const K& key, V& value) {
        std::shared_lock<std::mutex> lock(mutex_); // Lock for reading
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            return true;
        }
        return false; // Key not found
    }

    void printCache() {
        std::scoped_lock<std::mutex> lock(mutex_); // Lock for reading
        for (const auto& entry : cacheList) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
    }

private:
    size_t maxSize;
    std::deque<std::pair<K, V>> cacheList; 
    std::unordered_map<K, typename std::deque<std::pair<K, V>>::iterator> cacheMap; 

    mutable std::mutex mutex_; // Mutex for thread safety
};

// Example usage
int main() {
    Cache<int, std::string> cache(10);

    // Populate cache in separate threads
    std::thread producer1([&cache]() {
        for (int i = 1; i <= 5; ++i) {
            cache.put(i, "Value " + std::to_string(i));
        }
    });

    std::thread producer2([&cache]() {
        for (int i = 6; i <= 10; ++i) {
            cache.put(i, "Value " + std::to_string(i));
        }
    });

    std::thread consumer([&cache]() {
        std::string value;
        for (int i = 1; i <= 10; ++i) {
            if (cache.get(i, value)) {
                std::cout << "Found key " << i << ": " << value << std::endl;
            } else {
                std::cout << "Key " << i << " not found." << std::endl;
            }
        }
    });

    producer1.join();
    producer2.join();
    consumer.join();

    std::cout << "Final Cache State:" << std::endl;
    cache.printCache();

    return 0;
}