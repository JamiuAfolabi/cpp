# include <iostream>
# include <unordered_map>
# include <deque>

template<typename K, typename V>
class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize) {}

    void put(const K& key, const V& value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Update the value of the existing entry in the ring buffer
            cacheMap[key]->second = value;
            return;
        }

        if (cacheList.size() == maxSize) {
            // Cache is full, remove the oldest entry (circular behavior)
            K oldestKey = cacheList.front().first;
            cacheList.pop_front();
            cacheMap.erase(oldestKey);
        }

        // Insert the new entry at the back of the list (least recently used position)
        cacheList.push_back(std::make_pair(key, value));
        cacheMap[key] = --cacheList.end(); // The iterator to the new entry
    }

    bool get(const K& key, V& value) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            // Move the accessed entry to the back (most recently used position)
            cacheList.push_back(*it->second);
            cacheList.erase(it->second);
            cacheMap[key] = --cacheList.end();
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
    std::deque<std::pair<K, V>> cacheList; // To maintain the order of elements (ring buffer)
    std::unordered_map<K, typename std::deque<std::pair<K, V>>::iterator> cacheMap; // For fast access
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