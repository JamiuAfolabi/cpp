#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <variant>
#include <stdexcept>

// Custom exception classes for more informative error handling
class SerializationException : public std::runtime_error {
public:
    explicit SerializationException(const std::string& message) 
        : std::runtime_error("Serialization Error: " + message) {}
};

class DeserializationException : public std::runtime_error {
public:
    explicit DeserializationException(const std::string& message) 
        : std::runtime_error("Deserialization Error: " + message) {}
};

// Forward declarations
class SerializationContext;
class DeserializationContext;

// Type registry for handling polymorphic types
class TypeRegistry {
private:
    using CreateFunction = std::function<void*()>;
    static std::unordered_map<std::string, CreateFunction>& getRegistry() {
        static std::unordered_map<std::string, CreateFunction> registry;
        return registry;
    }

public:
    template<typename T>
    static void registerType(const std::string& typeName) {
        getRegistry()[typeName] = []() -> void* { return new T(); };
    }

    static void* createInstance(const std::string& typeName) {
        auto it = getRegistry().find(typeName);
        if (it != getRegistry().end()) {
            return it->second();
        }
        throw SerializationException("Unknown type: " + typeName);
    }
};

// Enhanced serializable interface
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(SerializationContext& context) const = 0;
    virtual void deserialize(DeserializationContext& context) = 0;
    virtual std::string getTypeName() const = 0;
    virtual ISerializable* clone() const = 0;
};

// Enhanced serialization context with array support
class SerializationContext {
private:
    std::stringstream& stream;
    std::unordered_map<const void*, size_t> pointerMap;
    size_t nextPointerId = 1;

public:
    explicit SerializationContext(std::stringstream& s) : stream(s) {}

    template<typename T>
    void write(const T& value) {
        if (!stream.write(reinterpret_cast<const char*>(&value), sizeof(T))) {
            throw SerializationException("Failed to write data to stream.");
        }
    }

    void writeString(const std::string& str) {
        size_t length = str.length();
        write(length);
        stream.write(str.c_str(), length);
    }

    template<typename T>
    void writePolymorphicPointer(const T* ptr) {
        if (ptr == nullptr) {
            writeString("");  // Empty string indicates null pointer
            return;
        }

        auto it = pointerMap.find(ptr);
        if (it != pointerMap.end()) {
            writeString("ref");  // Reference to existing object
            write(it->second);
        } else {
            size_t id = nextPointerId++;
            pointerMap[ptr] = id;
            writeString(ptr->getTypeName());
            write(id);
            ptr->serialize(*this);
        }
    }

    template<typename T>
    void writePointerArray(T** array, size_t size) {
        write(size);
        for (size_t i = 0; i < size; i++) {
            writePolymorphicPointer(array[i]);
        }
    }
};

// Enhanced deserialization context with array support
class DeserializationContext {
private:
    std::stringstream& stream;
    std::unordered_map<size_t, void*> pointerMap;

public:
    explicit DeserializationContext(std::stringstream& s) : stream(s) {}

    template<typename T>
    void read(T& value) {
        if (!stream.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            throw DeserializationException("Failed to read data from stream.");
        }
    }

    void readString(std::string& str) {
        size_t length;
        read(length);
        str.resize(length);
        stream.read(&str[0], length);
    }

    template<typename BaseT>
    BaseT* readPolymorphicPointer() {
        std::string typeName;
        readString(typeName);

        if (typeName.empty()) {
            return nullptr;
        }

        if (typeName == "ref") {
            size_t id;
            read(id);
            return static_cast<BaseT*>(pointerMap[id]);
        }

        size_t id;
        read(id);

        void* obj = TypeRegistry::createInstance(typeName);
        if (!obj) {
            throw DeserializationException("Failed to create instance for type: " + typeName);
        }

        BaseT* typedObj = static_cast<BaseT*>(obj);
        pointerMap[id] = typedObj;
        typedObj->deserialize(*this);
        return typedObj;
    }

    template<typename BaseT>
    void readPointerArray(BaseT*** arrayPtr, size_t& size) {
        read(size);
        if (size == 0) {
            *arrayPtr = nullptr;
            return;
        }

        *arrayPtr = new BaseT*[size];
        for (size_t i = 0; i < size; i++) {
            (*arrayPtr)[i] = readPolymorphicPointer<BaseT>();
        }
    }
};

// Example base class
class Animal : public ISerializable {
protected:
    std::string name;
    int age;

public:
    Animal() : age(0) {}
    Animal(const std::string& n, int a) : name(n), age(a) {}
    virtual ~Animal() = default;

    virtual void makeSound() const = 0;
    
    void serialize(SerializationContext& context) const override {
        context.writeString(name);
        context.write(age);
    }

    void deserialize(DeserializationContext& context) override {
        context.readString(name);
        context.read(age);
    }

    std::string getName() const { return name; }
    int getAge() const { return age; }
};

// Derived class: Dog
class Dog : public Animal {
private:
    std::string breed;

public:
    Dog() {}
    Dog(const std::string& n, int a, const std::string& b)
        : Animal(n, a), breed(b) {}

    void makeSound() const override {
        std::cout << "Woof!" << std::endl;
    }

    std::string getTypeName() const override {
        return "Dog";
    }

    ISerializable* clone() const override {
        return new Dog(*this);
    }

    void serialize(SerializationContext& context) const override {
        Animal::serialize(context);
        context.writeString(breed);
    }

    void deserialize(DeserializationContext& context) override {
        Animal::deserialize(context);
        context.readString(breed);
    }
};

// Derived class: Cat
class Cat : public Animal {
private:
    bool isIndoor;

public:
    Cat() {}
    Cat(const std::string& n, int a, bool indoor)
        : Animal(n, a), isIndoor(indoor) {}

    void makeSound() const override {
        std::cout << "Meow!" << std::endl;
    }

    std::string getTypeName() const override {
        return "Cat";
    }

    ISerializable* clone() const override {
        return new Cat(*this);
    }

    void serialize(SerializationContext& context) const override {
        Animal::serialize(context);
        context.write(isIndoor);
    }

    void deserialize(DeserializationContext& context) override {
        Animal::deserialize(context);
        context.read(isIndoor);
    }
};

// Example usage
int main() {
    // Register types
    TypeRegistry::registerType<Dog>("Dog");
    TypeRegistry::registerType<Cat>("Cat");

    // Create test data - pointer array
    const size_t size = 3;
    Animal** pointerArray = new Animal*[size] {
        new Dog("Rex", 5, "German Shepherd"),
        new Cat("Whiskers", 3, true),
        new Dog("Buddy", 2, "Golden Retriever")
    };

    // Serialize
    std::stringstream storage;
    try {
        SerializationContext serContext(storage);
        serContext.writePointerArray(pointerArray, size);
    } catch (const SerializationException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Clean up original pointer array
    for (size_t i = 0; i < size; i++) {
        delete pointerArray[i];
    }
    delete[] pointerArray;

    // Deserialize
    storage.seekg(0);
    DeserializationContext deserContext(storage);
    Animal** newPointerArray = nullptr;
    size_t newSize;

    try {
        deserContext.readPointerArray(&newPointerArray, newSize);
    } catch (const DeserializationException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Print results - pointer array
    std::cout << "Pointer Array Contents:\n";
    for (size_t i = 0; i < newSize; i++) {
        std::cout << "Animal: " << newPointerArray[i]->getName() 
                  << " (Type: " << newPointerArray[i]->getTypeName() 
                  << ", Age: " << newPointerArray[i]->getAge() << ")\n";
        newPointerArray[i]->makeSound();
    }

    // Clean up pointer array
    for (size_t i = 0; i < newSize; i++) {
        delete newPointerArray[i];
    }
    delete[] newPointerArray;

    return 0;
}