#include <iostream>
#include <string>
#include <vector>

template <typename K, typename V>
struct Pair {
    K key;
    V value;
    bool isDeleted; // Flag to indicate if this entry is deleted

    Pair(K k, const V &v) : key(k), value(v), isDeleted(false) {}
};

template <typename K, typename V>
class HashTable {
private:
    std::vector<Pair<K,V>*> table; // Using dynamic array of pointers to store key-value pairs
    int size; // Number of elements in the hash table
    int tableSize; // Current size of the table

    int hashFunction(K key) {
        return std::hash<K>()(key) % tableSize; // Simple modulus hash function using std::hash
    }

    void resize() {
        std::cout << "Resizing table from " << tableSize << " to " << tableSize * 2 << std::endl;
        int oldTableSize = tableSize;
        std::vector<Pair<K,V>*> oldTable = table;

        tableSize *= 2; // Double the size
        table.clear();
        table.resize(tableSize, nullptr);
        size = 0; // Reset size since we'll reinsert items

        // Rehash all existing items
        for (auto &pair : oldTable) {
            if (pair != nullptr && !pair->isDeleted) {
                insert(pair->key, pair->value); // Reinsert into the new table
                delete pair; // Clean up old pairs
            }
        }
    }

public:
    HashTable() : size(0), tableSize(10) {
        table.resize(tableSize, nullptr);
    }

    ~HashTable() {
        for (auto& pair : table) {
            delete pair; // Clean up dynamic memory
        }
    }

    void insert(K key, const V& value) {
        // Check load factor before insertion
        if (size >= 0.75 * tableSize) {
            resize(); // Resize the table if necessary
        }

        int index = hashFunction(key);
        while (true) {
            if (table[index] == nullptr || table[index]->isDeleted) {
                // Empty slot or deleted entry
                if (table[index] == nullptr) {
                    size++; // Increase size only for new insertions
                }
                table[index] = new Pair<K, V>(key, value);
                return;
            } else if (table[index]->key == key) {
                // Update value if the key already exists
                table[index]->value = value;
                table[index]->isDeleted = false; // Mark as valid
                return;
            }
            index = (index + 1) % tableSize; // Linear probing
        }
    }

    V get(K key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                return table[index]->value; // Return the value on match
            }
            index = (index + 1) % tableSize; // Continue probing
        }
        throw std::runtime_error("Key not found"); // Key not found
    }

    void remove(K key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                table[index]->isDeleted = true; // Mark the entry as deleted
                size--; // Decrease the size
                return;
            }
            index = (index + 1) % tableSize; // Continue probing
        }
        throw std::runtime_error("Key not found for deletion"); // Key not found
    }

    void display() {
        for (int i = 0; i < tableSize; ++i) {
            if (table[i] != nullptr) {
                if (table[i]->isDeleted) {
                    std::cout << "Index " << i << ": (deleted)\n";
                } else {
                    std::cout << "Index " << i << ": (" << table[i]->key << ", " << table[i]->value << ")\n";
                }
            } else {
                std::cout << "Index " << i << ": (empty)\n";
            }
        }
    }
};

// Main function demonstrating the use of the templated HashTable with different data types
int main() {
    // Example 1: Using integer keys and string values
    HashTable<int, std::string> intStringHashTable;
    intStringHashTable.insert(1, "One");
    intStringHashTable.insert(2, "Two");
    intStringHashTable.insert(3, "Three");
    intStringHashTable.display();

    std::cout << "Get key 2: " << intStringHashTable.get(2) << std::endl;
    intStringHashTable.remove(2);
    intStringHashTable.display();

    // Example 2: Using string keys and double values
    HashTable<std::string, double> stringDoubleHashTable;
    stringDoubleHashTable.insert("pi", 3.14);
    stringDoubleHashTable.insert("e", 2.71);
    stringDoubleHashTable.insert("phi", 1.618);
    stringDoubleHashTable.display();

    std::cout << "Get key 'pi': " << stringDoubleHashTable.get("pi") << std::endl;
    stringDoubleHashTable.remove("e");
    stringDoubleHashTable.display();

    return 0;
}