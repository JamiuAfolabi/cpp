#include <iostream>
#include <string>
#include <vector>

#define INITIAL_TABLE_SIZE 10 // Starting size of the hash table

struct Pair {
    int key;
    std::string value;
    bool isDeleted; // Flag to indicate if this entry is deleted

    Pair(int k, const std::string &v) : key(k), value(v), isDeleted(false) {}
};

class HashTable {
private:
    std::vector<Pair*> table; // Using dynamic array of pointers to store key-value pairs
    int size; // Number of elements in the hash table
    int tableSize; // Current size of the table

    int hashFunction(int key) {
        return key % tableSize; // Simple modulus hash function
    }

    void resize() {
        std::cout << "Resizing table from " << tableSize << " to " << tableSize * 2 << std::endl;
        int oldTableSize = tableSize;
        std::vector<Pair*> oldTable = table;

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
    HashTable() : size(0), tableSize(INITIAL_TABLE_SIZE) {
        table.resize(tableSize, nullptr);
    }

    ~HashTable() {
        for (auto& pair : table) {
            delete pair; // Clean up dynamic memory
        }
    }

    void insert(int key, const std::string& value) {
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
                table[index] = new Pair(key, value);
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

    std::string get(int key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                return table[index]->value; // Return the value on match
            }
            index = (index + 1) % tableSize; // Continue probing
        }
        return ""; // Key not found
    }

    void remove(int key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                table[index]->isDeleted = true; // Mark the entry as deleted
                size--; // Decrease the size
                return;
            }
            index = (index + 1) % tableSize; // Continue probing
        }
        std::cout << "Key not found for deletion: " << key << std::endl;
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

int main() {
    HashTable hashTable;

    for (int i = 0; i < 15; ++i) {
        hashTable.insert(i, "Value " + std::to_string(i));
    }

    std::cout << "Display hash table after multiple insertions:" << std::endl;
    hashTable.display();

    std::cout << "Get key 5: " << hashTable.get(5) << std::endl;
    std::cout << "Removing key 5." << std::endl;
    hashTable.remove(5);

    std::cout << "Display hash table after deletion:" << std::endl;
    hashTable.display();
    
    std::cout << "Get key 5 after deletion: " << hashTable.get(5) << std::endl; // should be empty

    return 0;
}