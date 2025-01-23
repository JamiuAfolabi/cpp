#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <iomanip>


using json = nlohmann::json;

class Serializable {
public:
    virtual json ToJson() const = 0;
    virtual void FromJson(const json& j) = 0;
};

// Base class for financial instruments
class FinancialInstrument : public Serializable {
public:
    virtual ~FinancialInstrument() = default;
};

class Stock : public FinancialInstrument {
public:
    std::string symbol;
    double price;

    Stock() = default;
    Stock(const std::string& symbol, double price) : symbol(symbol), price(price) {}

    json ToJson() const override {
        json j;
        j["symbol"] = symbol;
        j["price"] = price;
        return j;
    }

    void FromJson(const json& j) override {
        symbol = j["symbol"].get<std::string>();
        price = j["price"].get<double>();
    }
};

template<typename T>
class FinancialInstrumentHolder {
private:
    T instrument;

public:
    FinancialInstrumentHolder(const T& instrument) : instrument(instrument) {}

    json Serialize() const {
        return instrument.ToJson();
    }

    void Deserialize(const json& data) {
        instrument.FromJson(data);
    }
};

int main() {
    // Example usage:
    Stock stock("AAPL", 150.0);
    Option option("AAPL", 155.0, "2023-12-01");
    Future future("Gold", 100.0);

    FinancialInstrumentHolder<Stock> stockHolder(stock);
    FinancialInstrumentHolder<Option> optionHolder(option);
    FinancialInstrumentHolder<Future> futureHolder(future);

    // Serialize to JSON
    json serializedStock = stockHolder.Serialize();
    json serializedOption = optionHolder.Serialize();
    json serializedFuture = futureHolder.Serialize();

    std::cout << "Serialized Stock JSON: " << std::endl << serializedStock.dump(2) << std::endl;
    std::cout << "Serialized Option JSON: " << std::endl << serializedOption.dump(2) << std::endl;
    std::cout << "Serialized Future JSON: " << std::endl << serializedFuture.dump(2) << std::endl;

    // Deserialize from JSON
    Stock newStock;
    Option newOption;
    Future newFuture;

    FinancialInstrumentHolder<Stock> newStockHolder;
    FinancialInstrumentHolder<Option> newOptionHolder;
    FinancialInstrumentHolder<Future> newFutureHolder;

    newStockHolder.Deserialize(serializedStock);
    newOptionHolder.Deserialize(serializedOption);
    newFutureHolder.Deserialize(serializedFuture);

    return 0;
}