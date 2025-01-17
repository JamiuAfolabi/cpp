#include <iostream>
#include <string>
#include <vector>

#define TABLE_SIZE 10 // You can adjust this value for a larger/smaller table

struct Pair {
    int key;
    std::string value;
    Pair(int k, const std::string &v) : key(k), value(v) {}
};

class HashTable {
private:
    std::vector<Pair*> table; // Using dynamic array of pointers to store key-value pairs

    int hashFunction(int key) {
        return key % TABLE_SIZE; // Simple modulus hash function
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE, nullptr);
    }

    ~HashTable() {
        for (auto& pair : table) {
            delete pair; // Clean up dynamic memory
        }
    }

    void insert(int key, const std::string& value) {
        int index = hashFunction(key);
        // Create a new pair if the spot is empty
        if (table[index] == nullptr) {
            table[index] = new Pair(key, value);
        } else {
            // Handle collision with linear probing
            while (table[index] != nullptr) {
                if (table[index]->key == key) {
                    // Update value if the key already exists
                    table[index]->value = value;
                    return;
                }
                index = (index + 1) % TABLE_SIZE; // Linear probing
            }
            table[index] = new Pair(key, value); // Insert new pair
        }
    }

    std::string get(int key) {
        int index = hashFunction(key);
        // Linear probing to search for the key
        while (table[index] != nullptr) {
            if (table[index]->key == key) {
                return table[index]->value; // Return the value on match
            }
            index = (index + 1) % TABLE_SIZE; // Continue probing
        }
        return ""; // Key not found
    }

    void remove(int key) {
        int index = hashFunction(key);
        // Linear probing to find the key to delete
        while (table[index] != nullptr) {
            if (table[index]->key == key) {
                delete table[index]; // Delete the Pair
                table[index] = nullptr; // Remove reference
                // Optionally, you can rehash the following elements, but for simplicity, we'll skip that
                return;
            }
            index = (index + 1) % TABLE_SIZE; // Continue probing
        }
        std::cout << "Key not found for deletion: " << key << std::endl;
    }

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