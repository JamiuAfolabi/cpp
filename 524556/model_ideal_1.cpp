#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>
#include <string>

// Forward declarations
class SerializationContext;
class DeserializationContext;

// Interface for serializable objects
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(SerializationContext& context) const = 0;
    virtual void deserialize(DeserializationContext& context) = 0;
};

// Context class for handling serialization
class SerializationContext {
private:
    std::stringstream& stream;
    std::unordered_map<const void*, size_t> pointerMap;
    size_t nextPointerId = 1;

public:
    explicit SerializationContext(std::stringstream& s) : stream(s) {}

    template<typename T>
    void write(const T& value) {
        stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    void writeString(const std::string& str) {
        size_t length = str.length();
        write(length);
        stream.write(str.c_str(), length);
    }

    template<typename T>
    void writePointer(const T* ptr) {
        if (ptr == nullptr) {
            write(static_cast<size_t>(0));
            return;
        }

        auto it = pointerMap.find(ptr);
        if (it != pointerMap.end()) {
            write(it->second);  // Write existing ID
        } else {
            size_t id = nextPointerId++;
            pointerMap[ptr] = id;
            write(id);
            ptr->serialize(*this);
        }
    }

    template<typename T>
    void writeArray(const T* arr, size_t size) {
        write(size);
        for (size_t i = 0; i < size; ++i) {
            write(arr[i]);
        }
    }
};

// Context class for handling deserialization
class DeserializationContext {
private:
    std::stringstream& stream;
    std::unordered_map<size_t, void*> pointerMap;

public:
    explicit DeserializationContext(std::stringstream& s) : stream(s) {}

    template<typename T>
    void read(T& value) {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }

    void readString(std::string& str) {
        size_t length;
        read(length);
        str.resize(length);
        stream.read(&str[0], length);
    }

    template<typename T>
    T* readPointer() {
        size_t id;
        read(id);
        
        if (id == 0) {
            return nullptr;
        }

        auto it = pointerMap.find(id);
        if (it != pointerMap.end()) {
            return static_cast<T*>(it->second);
        }

        T* obj = new T();
        pointerMap[id] = obj;
        obj->deserialize(*this);
        return obj;
    }

    template<typename T>
    void readArray(T*& arr, size_t& size) {
        read(size);
        arr = new T[size];
        for (size_t i = 0; i < size; ++i) {
            read(arr[i]);
        }
    }
};

// Example usage with a complex data structure
class Node : public ISerializable {
public:
    int data;
    std::string name;
    Node* next;
    std::vector<int> values;

    Node() : data(0), next(nullptr) {}
    Node(int d, const std::string& n) : data(d), name(n), next(nullptr) {}

    void serialize(SerializationContext& context) const override {
        context.write(data);
        context.writeString(name);
        context.writePointer(next);
        
        size_t size = values.size();
        context.write(size);
        for (const auto& val : values) {
            context.write(val);
        }
    }

    void deserialize(DeserializationContext& context) override {
        context.read(data);
        context.readString(name);
        next = context.readPointer<Node>();
        
        size_t size;
        context.read(size);
        values.resize(size);
        for (size_t i = 0; i < size; ++i) {
            context.read(values[i]);
        }
    }
};

// Example demonstration
int main() {
    // Create a circular linked list with array data
    Node* head = new Node(1, "First");
    head->values = {10, 20, 30};
    
    Node* second = new Node(2, "Second");
    second->values = {40, 50, 60};
    
    Node* third = new Node(3, "Third");
    third->values = {70, 80, 90};
    
    head->next = second;
    second->next = third;
    third->next = head;  // Creating circular reference

    // Serialize
    std::stringstream storage;
    SerializationContext serContext(storage);
    serContext.writePointer(head);

    // Clear original data
    delete third;
    delete second;
    delete head;

    // Deserialize
    storage.seekg(0);
    DeserializationContext deserContext(storage);
    Node* newHead = deserContext.readPointer<Node>();

    // Verify the circular reference and data
    std::cout << "Original node: " << newHead->data << " " << newHead->name << "\n";
    std::cout << "Second node: " << newHead->next->data << " " << newHead->next->name << "\n";
    std::cout << "Third node: " << newHead->next->next->data << " " << newHead->next->next->name << "\n";
    std::cout << "Circular reference check: " << newHead->next->next->next->data << "\n";

    // Print array data from first node
    std::cout << "First node values: ";
    for (int val : newHead->values) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // Cleanup
    Node* current = newHead;
    Node* first = newHead;
    do {
        Node* temp = current;
        current = current->next;
        delete temp;
    } while (current != first);

    return 0;
}