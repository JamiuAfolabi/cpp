#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T>
struct Serializable {
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual std::unique_ptr<T> deserialize(const std::string& data) const = 0;

    static std::string typeName() { return typeid(T).name(); }
};

template <typename T>
struct Message : Serializable<T> {
    std::string serialize() const override {
        std::string data;
        // Implement serialization logic here (e.g., using std::stringstream)
        return data;
    }

    std::unique_ptr<T> deserialize(const std::string& data) const override {
        std::unique_ptr<T> result(new T());
        // Implement deserialization logic here (e.g., using std::stringstream)
        return result;
    }
};

struct Stock : Message<Stock> {
    std::string symbol;
    double price;
    int volume;
};

struct Option : Message<Option> {
    std::string underlying;
    double strike;
    std::string expiration;
    bool call;
};

struct Future : Message<Future> {
    std::string underlying;
    double settlementPrice;
    std::string expiration;
};

template <typename T>
std::string serializeMessage(const T& message) {
    return message.serialize();
}

template <typename T>
std::unique_ptr<T> deserializeMessage(const std::string& data) {
    return T::deserialize(data);
}

int main() {
    Stock stock;
    stock.symbol = "AAPL";
    stock.price = 162.5;
    stock.volume = 1000;

    std::string stockData = serializeMessage(stock);

    auto deserializedStock = deserializeMessage<Stock>(stockData);

    return 0;
}
