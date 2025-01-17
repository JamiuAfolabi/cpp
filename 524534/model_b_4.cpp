#include <iostream>
#include <string>
#include <vector>

#define INITIAL_TABLE_SIZE 10

template <typename Key, typename Value>
struct Pair {
    Key key;
    Value value;
    bool isDeleted;

    Pair(Key k, Value v) : key(k), value(v), isDeleted(false) {}
};

template <typename Key, typename Value>
class HashTable {
private:
    std::vector<Pair<Key, Value>*> table;
    int size;
    int tableSize;

    int hashFunction(Key key) {
        return key % tableSize;
    }

    void resize() {
        std::cout << "Resizing table from " << tableSize << " to " << tableSize * 2 << std::endl;
        int oldTableSize = tableSize;
        std::vector<Pair<Key, Value>*> oldTable = table;

        tableSize *= 2;
        table.clear();
        table.resize(tableSize, nullptr);
        size = 0;

        for (auto &pair : oldTable) {
            if (pair != nullptr && !pair->isDeleted) {
                insert(pair->key, pair->value);
                delete pair;
            }
        }
    }

public:
    HashTable() : size(0), tableSize(INITIAL_TABLE_SIZE) {
        table.resize(tableSize, nullptr);
    }

    ~HashTable() {
        for (auto& pair : table) {
            delete pair;
        }
    }

    void insert(Key key, Value value) {
        if (size >= 0.75 * tableSize) {
            resize();
        }

        int index = hashFunction(key);
        while (true) {
            if (table[index] == nullptr || table[index]->isDeleted) {
                if (table[index] == nullptr) {
                    size++;
                }
                table[index] = new Pair<Key, Value>(key, value);
                return;
            } else if (table[index]->key == key) {
                table[index]->value = value;
                table[index]->isDeleted = false;
                return;
            }
            index = (index + 1) % tableSize;
        }
    }

    Value get(Key key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                return table[index]->value;
            }
            index = (index + 1) % tableSize;
        }
        return Value(); // Return default value for the type
    }

    void remove(Key key) {
        int index = hashFunction(key);
        while (table[index] != nullptr) {
            if (!table[index]->isDeleted && table[index]->key == key) {
                table[index]->isDeleted = true;
                size--;
                return;
            }
            index = (index + 1) % tableSize;
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
    {
        HashTable<std::string, int> strToIntTable;

        strToIntTable.insert("apple", 2);
        strToIntTable.insert("banana", 5);
        strToIntTable.insert("orange", 4);

        std::cout << "Display string-to-int hash table:" << std::endl;
        strToIntTable.display();

        std::cout << "Get value for 'banana': " << strToIntTable.get("banana") << std::endl;
        std::cout << "Remove 'orange'.\n";
        strToIntTable.remove("orange");

        std::cout << "Display string-to-int hash table after deletion:" << std::endl;
        strToIntTable.display();
    }

    {
        HashTable<double, std::complex<double>> doubleToComplexTable;

        doubleToComplexTable.insert(3.14, std::complex<double>(1.0, 2.0));
        doubleToComplexTable.insert(2.72, std::complex<double>(4.0, 5.0));

        std::cout << "\nDisplay double-to-complex hash table:" << std::endl;
        doubleToComplexTable.display();

        std::cout << "Get value for 3.14: " << doubleToComplexTable.get(3.14) << std::endl;
        std::cout << "Remove 2.72.\n";
        doubleToComplexTable.remove(2.72);

        std::cout << "Display double-to-complex hash table after deletion:" << std::endl;
        doubleToComplexTable.display();
    }

    return 0;
}