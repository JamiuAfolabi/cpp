#include <iostream>
#include <string>
#include <vector>

#define LOAD_FACTOR_THRESHOLD 0.75

struct Pair {
    int key;
    std::string value;
    Pair(int k, const std::string& v) : key(k), value(v) {}
};

class HashTable {
private:
    std::vector<Pair*> table;
    size_t size = 0; // Count of elements in the hash table

    int hashFunction(int key) {
        return key % table.size();
    }

    void resize() {
        // Double the table size
        std::vector<Pair*> oldTable = table; // Store old table to rehash elements
        table.resize(table.size() * 2);
        size = 0; // Reset the element count

        // Rehash all existing elements into the new table
        for (auto& pair : oldTable) {
            if (pair != nullptr) {
                int index = hashFunction(pair->key);
                insert(pair->key, pair->value);
            }
        }
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE, nullptr);
    }

    ~HashTable() {
        for (auto& pair : table) {
            delete pair;
        }
    }

    void insert(int key, const std::string& value) {
        int index = hashFunction(key);
        if (table[index] == nullptr) {
            table[index] = new Pair(key, value);
            size++;
        } else {
            while (table[index] != nullptr) {
                if (table[index]->key == key) {
                    table[index]->value = value;
                    return;
                }
                index = (index + 1) % table.size();
            }
            table[index] = new Pair(key, value);
            size++;
        }

        // Check if load factor exceeds threshold and resize if needed
        double loadFactor = static_cast<double>(size) / table.size();
        if (loadFactor > LOAD_FACTOR_THRESHOLD) {
            resize();
        }
    }
 
   // Rest of the code remains the same...
   void display() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i] != nullptr) {
                std::cout << "Index " << i << ": (" << table[i]->key << ", " << table[i]->value << ")\n";
            } else {
                std::cout << "Index " << i << ": (empty)\n";
            }
        }
    }

};

int main() {
    HashTable hashTable;

    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(11, "Eleven"); // This will collide with key 1, demonstrate collision handling
    
    std::cout << "Display hash table:" << std::endl;
    hashTable.display();

    std::cout << "Get key 1: " << hashTable.get(1) << std::endl;
    std::cout << "Get key 2: " << hashTable.get(2) << std::endl;
    std::cout << "Get key 11: " << hashTable.get(11) << std::endl;
    std::cout << "Get key 3 (not found): " << hashTable.get(3) << std::endl; // should return ""

    std::cout << "Removing key 2." << std::endl;
    hashTable.remove(2);
    
    std::cout << "Display hash table after deletion:" << std::endl;
    hashTable.display();

    return 0;
}
