#include <iostream>
#include <unordered_map>
#include <queue>

template<typename Key, typename Value>
class LRUCache {

    using Entry = std::pair<Key, Value>;
    using AccessOrderQueue = std::queue<Key>;

public:
    explicit LRUCache(int capacity) : capacity_(capacity) {
        // Set capacity to 10 if it is less than or equal to 0
        capacity_ = std::max(capacity, 1);
    }

    void insert(const Key& key, const Value& value) {
        auto it = cache_.find(key);

        if (it != cache_.end()) {
            // Key already exists in the cache, update the value and move it to the front of the queue
            cache_.at(key) = value;
            accessOrderQueue_.erase(accessOrderQueue_.find(key));
            accessOrderQueue_.push(key);
        } else {
            if (cache_.size() == capacity_) {
                // Cache is full, remove the least recently used item
                const Key leastRecentlyUsedKey = accessOrderQueue_.front();
                accessOrderQueue_.pop();
                cache_.erase(leastRecentlyUsedKey);
            }

            // Insert the new key-value pair to the cache and the front of the queue
            cache_[key] = value;
            accessOrderQueue_.push(key);
        }
    }

    Value get(const Key& key) const {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            throw std::runtime_error("Key not found in the cache.");
        }

        // Move the accessed key to the front of the queue
        accessOrderQueue_.erase(accessOrderQueue_.find(key));
        accessOrderQueue_.push(key);

        return it->second;
    }

    [[nodiscard]] int size() const { return cache_.size(); }

private:
    int capacity_;
    std::unordered_map<Key, Value> cache_;
    AccessOrderQueue accessOrderQueue_;
};
 
 int main() {
    LRUCache<int, int> cache(3);  // Cache with capacity 3

    cache.insert(1, 1);
    cache.insert(2, 2);
    cache.insert(3, 3);

    try {
        std::cout << "cache[1] = " << cache.get(1) << std::endl; // Output: cache[1] = 1
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    cache.insert(4, 4);  // Key 1 will be removed (LRU)

    try {
        std::cout << "cache[1] = " << cache.get(1) << std::endl; // Key 1 is not present now
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "cache[4] = " << cache.get(4) << std::endl; // Output: cache[4] = 4
    std::cout << "cache size: " << cache.size() << std::endl; // Output: cache size: 3

    return 0;
}  