#include <iostream>
#include <unordered_map>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

template<typename K, typename V>
class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize) {}

    void put(const K& key, const V& value) {
        {
            std::unique_lock<std::mutex> lock(cacheMutex);
            cacheFullCV.wait(lock, [this]() { return cacheList.size() < maxSize || !atCapacity; });

            if (cacheMap.find(key) != cacheMap.end()) {
                // Update the value of the existing entry in the ring buffer
                cacheMap[key]->second = value;
            } else {
                if (cacheList.size() == maxSize) {
                    // Cache is full, remove the oldest entry
                    K oldestKey = cacheList.front().first;
                    cacheList.pop_front();
                    cacheMap.erase(oldestKey);
                }
                cacheList.push_back(std::make_pair(key, value));
                cacheMap[key] = --cacheList.end();
            }
            atCapacity = cacheList.size() == maxSize;
        }
        cacheEmptyCV.notify_one();
    }

    bool get(const K& key, V& value) {
        {
            std::unique_lock<std::mutex> lock(cacheMutex);
            cacheEmptyCV.wait(lock, [this]() { return !cacheList.empty(); });

            auto it = cacheMap.find(key);
            if (it != cacheMap.end()) {
                cacheList.push_back(*it->second);
                cacheList.erase(it->second);
                cacheMap[key] = --cacheList.end();
                value = it->second->second;
                return true;
            }
        }
        return false; // Key not found
    }

    void printCache() const {
        {
            std::unique_lock<std::mutex> lock(cacheMutex);
            cacheEmptyCV.wait(lock, [this]() { return !cacheList.empty(); });

            for (const auto& entry : cacheList) {
                std::cout << entry.first << ": " << entry.second << std::endl;
            }
        }
    }

private:
    size_t maxSize;
    std::deque<std::pair<K, V>> cacheList;
    std::unordered_map<K, typename std::deque<std::pair<K, V>>::iterator> cacheMap;

    mutable std::mutex cacheMutex;
    std::condition_variable cacheEmptyCV;
    std::condition_variable cacheFullCV;
    bool atCapacity = false;
};

int main() {
    Cache<int, std::string> cache(3); // Create a cache with max size of 3

    // Start a few threads to simulate concurrent cache operations
    std::thread producer1([&]() {
        cache.put(1, "Value 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cache.put(2, "Value 2");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cache.put(3, "Value 3");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cache.put(4, "Value 4");
    });

    std::thread producer2([&]() {
        cache.put(5, "Value 5");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cache.put(6, "Value 6");
    });

    std::thread consumer([&]() {
        std::string value;
        if (cache.get(1, value)) {
            std::cout << "Retrieved Key 1: " << value << std::endl;
        }
        if (cache.get(2, value)) {
            std::cout << "Retrieved Key 2: " << value << std::endl;
        }
        if (cache.get(3, value)) {
            std::cout << "Retrieved Key 3: " << value << std::endl;
        }
        if (cache.get(4, value)) {
            std::cout << "Retrieved Key 4: " << value << std::endl;
        }
    });

    // Wait for threads to finish
    producer1.join();
    producer2.join();
    consumer.join();

    // Print the final cache content
    std::cout << "\nFinal Cache State:" << std::endl;
    cache.printCache();

    return 0;
}