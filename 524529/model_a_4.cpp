#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <exception>
#include <cassert>

using json = nlohmann::json;

class JsonSerializationException : public std::exception {
public:
    explicit JsonSerializationException(const std::string& message) : msg_(message) {}

    virtual const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

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
        try {
            if (!data.contains("symbol") || !data.contains("price")) {
                throw JsonSerializationException("Missing required fields in Stock JSON");
            }
            if (!data["symbol"].is_string() || !data["price"].is_number()) {
                throw JsonSerializationException("Invalid types for fields in Stock JSON");
            }

            symbol = data["symbol"].get<std::string>();
            price = data["price"].get<double>();
        } catch (const std::exception& e) {
            throw JsonSerializationException(std::string("Error deserializing Stock: ") + e.what());
        }
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
        try {
            if (!data.contains("symbol") || !data.contains("strikePrice") || !data.contains("expiryDate")) {
                throw JsonSerializationException("Missing required fields in Option JSON");
            }
            if (!data["symbol"].is_string() || !data["strikePrice"].is_number() || !data["expiryDate"].is_string()) {
                throw JsonSerializationException("Invalid types for fields in Option JSON");
            }

            symbol = data["symbol"].get<std::string>();
            strikePrice = data["strikePrice"].get<double>();
            expiryDate = data["expiryDate"].get<std::string>();
        } catch (const std::exception& e) {
            throw JsonSerializationException(std::string("Error deserializing Option: ") + e.what());
        }
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
        try {
            if (!data.contains("commodity") || !data.contains("contractSize")) {
                throw JsonSerializationException("Missing required fields in Future JSON");
            }
            if (!data["commodity"].is_string() || !data["contractSize"].is_number()) {
                throw JsonSerializationException("Invalid types for fields in Future JSON");
            }

            commodity = data["commodity"].get<std::string>();
            contractSize = data["contractSize"].get<double>();
        } catch (const std::exception& e) {
            throw JsonSerializationException(std::string("Error deserializing Future: ") + e.what());
        }
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
    // Create and serialize financial instruments
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

    // Deserialize from JSON with error handling
    try {
        Stock newStock;
        newStock.DeserializeFromJson(serializedStock);

        Option newOption;
        newOption.DeserializeFromJson(serializedOption);

        Future newFuture;
        newFuture.DeserializeFromJson(serializedFuture);

        // Assertions to verify the correctness of deserialization
        assert(newStock.symbol == "AAPL");
        assert(newStock.price == 150.0);
        
        assert(newOption.symbol == "AAPL");
        assert(newOption.strikePrice == 155.0);
        assert(newOption.expiryDate == "2023-12-01");
        
        assert(newFuture.commodity == "Gold");
        assert(newFuture.contractSize == 100.0);

        std::cout << "All assertions passed!" << std::endl;

    } catch (const JsonSerializationException& e) {
        std::cerr << "Deserialization error: " << e.what() << std::endl;
    }

    return 0;
}