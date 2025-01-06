#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>

template <typename K, typename V>
class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize), startIndex(0) { entries.reserve(maxSize); }

    void put(const K& key, const V& value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Key exists, update its value
            auto index = cacheMap[key] - entries.begin();
            entries[index].second = value;
            moved_to_front.push_front(index);
            return;
        }

        if (entries.size() == maxSize) {
            // Overwrite the oldest entry
            auto oldest_index = moved_to_front.back();
            moved_to_front.pop_back();

            entries[oldest_index] = {key, value};
            cacheMap[key] = entries.begin() + oldest_index;
        } else {
            entries.push_back({key, value});
            cacheMap[key] = --entries.end();
        }
        cacheMap.erase(moved_to_front.front());
        moved_to_front.pop_front();

        moved_to_front.push_front(entries.size() - 1);
    }

    bool get(const K& key, V& value) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            auto index = *it - entries.begin();
            value = entries[index].second;
	    moved_to_front.push_front(index);
            return true;
        }
        return false;
    }
 
    void printCache() const {
	for (size_t i = startIndex; i < entries.size() || i < startIndex; i++) {
            const auto& entry = entries[(i + startIndex) % entries.size()];
            std::cout << entry.first << ": " << entry.second << std::endl;
	}
    }

private:
    size_t maxSize;
    size_t startIndex;
    std::vector<std::pair<K, V>> entries;
    std::unordered_map<K, typename std::vector<std::pair<K, V>>::iterator> cacheMap;
    std::deque<size_t> moved_to_front;
};

int main() {
    Cache<int, std::string> cache(10);

    // Add multiple entries
    for (int i = 1; i <= 15; ++i) {
        cache.put(i, std::to_string(i) + ":value");
    }

    std::cout << "Cache using ring buffer:" << std::endl;
    cache.printCache();

    return 0;
}
