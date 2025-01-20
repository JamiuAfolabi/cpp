#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>

class Serializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string &data) = 0;
    virtual ~Serializable() {}
};

class SerializeManager {
private:
    std::unordered_map<const void*, size_t> addresses;
    size_t nextId = 0;

public:
    size_t getOrAddAddress(const void *ptr) {
        auto it = addresses.find(ptr);
        if (it != addresses.end()) {
            return it->second;
        } else {
            addresses[ptr] = nextId;
            return nextId++;
        }
    }

    void clear() {
        addresses.clear();
        nextId = 0;
    }
};

class Example : public Serializable {
public:
    int a;
    std::vector<int> arr;
    std::shared_ptr<Example> child;

    Example(int val) : a(val), child(nullptr) {}

    std::string serialize() const override {
        std::ostringstream oss;
        oss << "{ \"a\": " << a << ", \"arr\": [";
        for (size_t i = 0; i < arr.size(); ++i) {
            oss << arr[i];
            if (i < arr.size() - 1) {
                oss << ", ";
            }
        }
        oss << "], \"child\": " << (child ? std::to_string(child->serializeId) : "null") << " }";
        return oss.str();
    }

    void deserialize(const std::string &data) override {
        // Example of deserialization (not complete, just a placeholder)
        // In a complete implementation, you'd parse the JSON-like string
        std::cout << "Deserializing: " << data << std::endl;
    }

    size_t serializeId;
};

// Example of user-defined serialization function
void serializeExample(const Example& example, SerializeManager &manager) {
    example.serializeId = manager.getOrAddAddress(&example);
}

int main() {
    SerializeManager manager;
    
    // Create a circular reference
    auto root = std::make_shared<Example>(10);
    root->arr = {1, 2, 3};
    root->child = std::make_shared<Example>(20);
    root->child->child = root; // Circular reference

    // Serialize
    serializeExample(*root, manager);
    std::cout << "Serialized: " << root->serialize() << std::endl;

    // Clear the manager for the next use
    manager.clear();

    return 0;
}