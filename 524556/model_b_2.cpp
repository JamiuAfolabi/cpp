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

    // Function to get the dynamic type name
    virtual std::string getTypeName() const = 0;
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
    std::unordered_map<std::string, std::function<ISerializable*()>> typeCreators;


public:
    explicit DeserializationContext(std::stringstream& s) : stream(s) {}

    template<typename T>
    void registerTypeCreator() {
        typeCreators[typeid(T).name()] = []() { return new T(); };
    }

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

    ISerializable* readObject() {
        std::string typeName;
        readString(typeName);

        auto it = typeCreators.find(typeName);
        if (it == typeCreators.end()) {
            std::cerr << "Error: Deserializing unknown type: " << typeName << std::endl;
            return nullptr;
        }

        ISerializable* obj = it->second();
        obj->deserialize(*this);
        return obj;
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


// Example usage with a complex data structure and inheritance
class BaseShape : public ISerializable {
public:
    virtual void serialize(SerializationContext& context) const override {
        context.writeString(getTypeName());
    }

    virtual void deserialize(DeserializationContext& context) override {
        // Derived classes will implement this
    }

    virtual std::string getTypeName() const override {
        return typeid(*this).name();
    }

    virtual ~BaseShape() = default;
};

class Circle : public BaseShape {
public:
    double radius;

    Circle() = default;
    Circle(double r) : radius(r) {}

    void serialize(SerializationContext& context) const override {
        BaseShape::serialize(context);
        context.write(radius);
    }

    void deserialize(DeserializationContext& context) override {
        context.read(radius);
    }
};

class Rectangle : public BaseShape {
public:
    double length;
    double width;

    Rectangle() = default;
    Rectangle(double l, double w) : length(l), width(w) {}

    void serialize(SerializationContext& context) const override {
        BaseShape::serialize(context);
        context.write(length);
        context.write(width);
    }

    void deserialize(DeserializationContext& context) override {
        context.read(length);
        context.read(width);
    }
};

// Example demonstration
int main() {
    std::stringstream storage;
    SerializationContext serContext(storage);

    {
        std::shared_ptr<BaseShape> circle = std::make_shared<Circle>(5.0);
        std::shared_ptr<BaseShape> rect = std::make_shared<Rectangle>(10.0, 20.0);

        serContext.writePointer(circle.get());
        serContext.writePointer(rect.get());
    }

    storage.seekg(0);
    DeserializationContext deserContext(storage);
    deserContext.registerTypeCreator<Circle>();
    deserContext.registerTypeCreator<Rectangle>();

    std::shared_ptr<BaseShape> circle2 = std::dynamic_pointer_cast<Circle>(deserContext.readPointer<BaseShape>());
    std::shared_ptr<BaseShape> rect2 = std::dynamic_pointer_cast<Rectangle>(deserContext.readPointer<BaseShape>());

    if (circle2) {
        std::cout << "Circle radius: " << circle2->radius << std::endl;
    }

    if (rect2) {
        std::cout << "Rectangle length: " << rect2->length << ", width: " << rect2->width << std::endl;
    }

    return 0;
}