#include <iostream>
#include <string>
#include <cstdlib> // For malloc and free

using namespace std;

// Define a structure for key-value pairs
struct KeyValuePair {
    int key;
    string value;
    KeyValuePair* next;
};

class HashTable {
private:
    int size;
    KeyValuePair** buckets;

    // Simple hash function: Modulo of the key with the size of the hash table
    int hashFunction(int key) {
        return key % size;
    }

public:
    // Constructor: Initialize the hash table with a given size
    HashTable(int size) {
        this->size = size;
        buckets = new KeyValuePair*[size];
        for (int i = 0; i < size; i++) {
            buckets[i] = nullptr;
        }
    }

    // Destructor: Free the allocated memory
    ~HashTable() {
        for (int i = 0; i < size; i++) {
            KeyValuePair* temp = buckets[i];
            while (temp != nullptr) {
                KeyValuePair* next = temp->next;
                delete temp;
                temp = next;
            }
        }
        delete[] buckets;
    }

    // Insert a key-value pair into the hash table
    void insert(int key, string value) {
        int index = hashFunction(key);
        KeyValuePair* newPair = new KeyValuePair;
        newPair->key = key;
        newPair->value = value;
        newPair->next = buckets[index];
        buckets[index] = newPair;
    }

    // Search for a value given a key in the hash table
    string get(int key) {
        int index = hashFunction(key);
        KeyValuePair* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return ""; // Key not found
    }

    // Delete a key-value pair from the hash table
    void remove(int key) {
        int index = hashFunction(key);
        KeyValuePair* prev = nullptr;
        KeyValuePair* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    buckets[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }

        // Key not found
    }
};


int main() {
    const int TABLE_SIZE = 10;
    HashTable hashTable(TABLE_SIZE);

    hashTable.insert(1, "apple");
    hashTable.insert(2, "banana");
    hashTable.insert(3, "orange");
    hashTable.insert(4, "mango");
    hashTable.insert(5, "grape");

    cout << "Key 3: " << hashTable.get(3) << endl;
    hashTable.remove(3);
    cout << "Key 3 after remove: " << hashTable.get(3) << endl;

    return 0;
}
