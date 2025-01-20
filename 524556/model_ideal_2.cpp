#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeinfo>
#include <sstream>

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
        return nullptr;
    }
};

// Enhanced serializable interface
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(SerializationContext& context) const = 0;
    virtual void deserialize(DeserializationContext& context) = 0;
    virtual std::string getTypeName() const = 0;
};

// Enhanced serialization context
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
            writeString(ptr->getTypeName());  // Write actual type name
            write(id);
            ptr->serialize(*this);
        }
    }
};

// Enhanced deserialization context
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
            throw std::runtime_error("Unknown type: " + typeName);
        }

        BaseT* typedObj = static_cast<BaseT*>(obj);
        pointerMap[id] = typedObj;
        typedObj->deserialize(*this);
        return typedObj;
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

    // Create objects
    std::vector<Animal*> animals;
    animals.push_back(new Dog("Rex", 5, "German Shepherd"));
    animals.push_back(new Cat("Whiskers", 3, true));
    animals.push_back(new Dog("Buddy", 2, "Golden Retriever"));

    // Serialize
    std::stringstream storage;
    SerializationContext serContext(storage);
    
    size_t count = animals.size();
    serContext.write(count);
    for (const Animal* animal : animals) {
        serContext.writePolymorphicPointer(animal);
    }

    // Clean up original objects
    for (Animal* animal : animals) {
        delete animal;
    }
    animals.clear();

    // Deserialize
    storage.seekg(0);
    DeserializationContext deserContext(storage);
    
    size_t newCount;
    deserContext.read(newCount);
    
    std::vector<Animal*> newAnimals;
    for (size_t i = 0; i < newCount; ++i) {
        Animal* animal = deserContext.readPolymorphicPointer<Animal>();
        newAnimals.push_back(animal);
    }

    // Verify deserialized objects
    for (const Animal* animal : newAnimals) {
        std::cout << "Animal: " << animal->getName() 
                  << " (Type: " << animal->getTypeName() 
                  << ", Age: " << animal->getAge() << ")\n";
        animal->makeSound();
    }

    // Cleanup
    for (Animal* animal : newAnimals) {
        delete animal;
    }

    return 0;
}