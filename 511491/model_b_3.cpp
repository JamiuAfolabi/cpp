#include <iostream>
#include <unordered_map>
#include <deque>
#include <mutex>
#include <condition_variable>

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