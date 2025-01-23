#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Serializable {
public:
    virtual json SerializeToJson() const = 0;
    virtual void DeserializeFromJson(const json& data) = 0;
};

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

    json SerializeToJson() const override {
        return json{
            {"type", "Stock"},
            {"symbol", symbol},
            {"price", price}
        };
    }

    void DeserializeFromJson(const json& data) override {
        symbol = data["symbol"].get<std::string>();
        price = data["price"].get<double>();
    }
};

class Option : public FinancialInstrument {
public:
    std::string symbol;
    double strikePrice;
    std::string expiryDate;

    Option() = default;
    Option(const std::string& symbol, double strikePrice, const std::string& expiryDate)
        : symbol(symbol), strikePrice(strikePrice), expiryDate(expiryDate) {}

    json SerializeToJson() const override {
        return json{
            {"type", "Option"},
            {"symbol", symbol},
            {"strikePrice", strikePrice},
            {"expiryDate", expiryDate}
        };
    }

    void DeserializeFromJson(const json& data) override {
        symbol = data["symbol"].get<std::string>();
        strikePrice = data["strikePrice"].get<double>();
        expiryDate = data["expiryDate"].get<std::string>();
    }
};

class Future : public FinancialInstrument {
public:
    std::string commodity;
    double contractSize;

    Future() = default;
    Future(const std::string& commodity, double contractSize) 
        : commodity(commodity), contractSize(contractSize) {}

    json SerializeToJson() const override {
        return json{
            {"type", "Future"},
            {"commodity", commodity},
            {"contractSize", contractSize}
        };
    }

    void DeserializeFromJson(const json& data) override {
        commodity = data["commodity"].get<std::string>();
        contractSize = data["contractSize"].get<double>();
    }
};

template<typename T>
class FinancialInstrumentHolder {
private:
    T instrument;

public:
    FinancialInstrumentHolder(const T& instrument) : instrument(instrument) {}

    json SerializeToJson() const {
        return instrument.SerializeToJson();
    }

    void DeserializeFromJson(const json& data) {
        instrument.DeserializeFromJson(data);
    }

    const T& getInstrument() const { return instrument; }
};

int main() {
    Stock stock("AAPL", 150.0);
    Option option("AAPL", 155.0, "2023-12-01");
    Future future("Gold", 100.0);

    FinancialInstrumentHolder<Stock> stockHolder(stock);
    FinancialInstrumentHolder<Option> optionHolder(option);
    FinancialInstrumentHolder<Future> futureHolder(future);

    // Serialize to JSON
    json serializedStock = stockHolder.SerializeToJson();
    json serializedOption = optionHolder.SerializeToJson();
    json serializedFuture = futureHolder.SerializeToJson();

    std::cout << "Serialized Stock: " << serializedStock.dump(2) << std::endl;
    std::cout << "Serialized Option: " << serializedOption.dump(2) << std::endl;
    std::cout << "Serialized Future: " << serializedFuture.dump(2) << std::endl;

    // Deserialize from JSON
    Stock newStock;
    newStock.DeserializeFromJson(serializedStock);
    Option newOption;
    newOption.DeserializeFromJson(serializedOption);
    Future newFuture;
    newFuture.DeserializeFromJson(serializedFuture);

    std::cout << "Deserialized Stock Symbol: " << newStock.symbol 
              << " Price: " << newStock.price << std::endl;
    std::cout << "Deserialized Option Symbol: " << newOption.symbol 
              << " Strike Price: " << newOption.strikePrice 
              << " Expiry Date: " << newOption.expiryDate << std::endl;
    std::cout << "Deserialized Future Commodity: " << newFuture.commodity 
              << " Contract Size: " << newFuture.contractSize << std::endl;

    return 0;
}