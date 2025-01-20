#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>

// Forward declarations
class SerializationContext;
class DeserializationContext;

// Interface for serializable objects
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(SerializationContext& context) const = 0;
    virtual void deserialize(DeserializationContext& context) = 0;
    virtual const std::type_index getType() const = 0; // New method to get object type
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

    void writeType(const std::type_index& type) { // Write type info
        write(type.hash_code());
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
            writeType(typeid(*ptr)); // Write type before serializing the object
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
    std::sstream& stream;
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

    std::type_index readType() { // Read type info
        size_t typeHash;
        read(typeHash);
        return std::type_index(typeHash);
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

        // Create the derived type
        std::type_index type = readType();
        void* obj = createObject(type);
        pointerMap[id] = obj;
        static_cast<ISerializable*>(obj)->deserialize(*this);
        return static_cast<T*>(obj);
    }

    void* createObject(const std::type_index& type) {
        if (type == typeid(DerivedA)) return new DerivedA();
        if (type == typeid(DerivedB)) return new DerivedB();
        // Add more derived types as needed

        return nullptr; // Handle unknown types safely
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

// Base class
class BaseNode : public ISerializable {
public:
    virtual ~BaseNode() = default;
    virtual const std::type_index getType() const override { return typeid(BaseNode); }
};

// Derived classes
class DerivedA : public BaseNode {
public:
    int valueA;
    
    DerivedA() : valueA(0) {}

    void serialize(SerializationContext& context) const override {
        context.write(valueA);
    }

    void deserialize(DeserializationContext& context) override {
        context.read(valueA);
    }

    const std::type_index getType() const override { return typeid(DerivedA); }
};

class DerivedB : public BaseNode {
public:
    std::string valueB;

    DerivedB() : valueB("") {}

    void serialize(SerializationContext& context) const override {
        context.writeString(valueB);
    }

    void deserialize(DeserializationContext& context) override {
        context.readString(valueB);
    }

    const std::type_index getType() const override { return typeid(DerivedB); }
};

int main() {
    std::stringstream storage;

    // Create polymorphic objects
    BaseNode* objects[] = { new DerivedA{15}, new DerivedB{"Hello"} };

    // Serialize
    SerializationContext serContext(storage);
    for (auto obj : objects) {
        serContext.writePointer(obj);
    }

    // Clear original objects
    for (auto obj : objects) {
        delete obj;
    }

    // Deserialize
    storage.seekg(0);
    DeserializationContext deserContext(storage);
    for (int i = 0; i < 2; ++i) {
        BaseNode* obj = deserContext.readPointer<BaseNode>();
        if (auto derivedA = dynamic_cast<DerivedA*>(obj)) {
            std::cout << "DerivedA value: " << derivedA->valueA << std::endl;
        } else if (auto derivedB = dynamic_cast<DerivedB*>(obj)) {
            std::cout << "DerivedB value: " << derivedB->valueB << std::endl;
        }
        delete obj; // Clean up allocated memory
    }

    return 0;
}