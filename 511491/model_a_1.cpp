#include <iostream>
#include <list>
#include <unordered_map>

template<typename K, typename V>
class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize) {}

    void put(const K& key, const V& value) {
        // Check if the key already exists
        if (cacheMap.find(key) != cacheMap.end()) {
            // Remove the existing entry from the list
            cacheList.erase(cacheMap[key]);
            cacheMap.erase(key);
        } else if (cacheList.size() == maxSize) {
            // Cache is full, remove the oldest entry
            K oldestKey = cacheList.back().first;
            cacheList.pop_back();
            cacheMap.erase(oldestKey);
        }

        // Insert the new entry at the front (most recently used)
        cacheList.push_front(std::make_pair(key, value));
        cacheMap[key] = cacheList.begin();
    }

    bool get(const K& key, V& value) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            // Move the accessed entry to the front (most recently used)
            cacheList.splice(cacheList.begin(), cacheList, it->second);
            value = it->second->second;
            return true;
        }
        return false; // Key not found
    }

    void printCache() const {
        for (const auto& entry : cacheList) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
    }

private:
    size_t maxSize;
    std::list<std::pair<K, V>> cacheList; // To maintain the order of elements
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cacheMap; // For fast access
};

// Example usage
int main() {
    Cache<int, std::string> cache(10);

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