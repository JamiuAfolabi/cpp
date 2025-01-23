#include <nlohmann/json.hpp> // Include the nlohmann/json header
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

class Stock : public FinancialInstrument {
public:
    std::string symbol;
    double price;

    Stock() = default;
    Stock(const std::string& symbol, double price) : symbol(symbol), price(price) {}

    std::string Serialize() const override {
        json j;
        j["type"] = "Stock"; // type identifier
        j["symbol"] = symbol;
        j["price"] = price;
        return j.dump(); // return JSON string
    }

    void Deserialize(const std::string& data) override {
        auto j = json::parse(data); // parse JSON
        symbol = j.at("symbol").get<std::string>();
        price = j.at("price").get<double>();
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

    std::string Serialize() const override {
        json j;
        j["type"] = "Option"; // type identifier
        j["symbol"] = symbol;
        j["strikePrice"] = strikePrice;
        j["expiryDate"] = expiryDate;
        return j.dump(); // return JSON string
    }

    void Deserialize(const std::string& data) override {
        auto j = json::parse(data);
        symbol = j.at("symbol").get<std::string>();
        strikePrice = j.at("strikePrice").get<double>();
        expiryDate = j.at("expiryDate").get<std::string>();
    }
};

class Future : public FinancialInstrument {
public:
    std::string commodity;
    double contractSize;

    Future() = default;
    Future(const std::string& commodity, double contractSize) : commodity(commodity), contractSize(contractSize) {}

    std::string Serialize() const override {
        json j;
        j["type"] = "Future"; // type identifier
        j["commodity"] = commodity;
        j["contractSize"] = contractSize;
        return j.dump(); // return JSON string
    }

    void Deserialize(const std::string& data) override {
        auto j = json::parse(data);
        commodity = j.at("commodity").get<std::string>();
        contractSize = j.at("contractSize").get<double>();
    }
};

int main() {
    Stock stock("AAPL", 150.0);
    Option option("AAPL", 155.0, "2023-12-01");
    Future future("Gold", 100.0);

    FinancialInstrumentHolder<Stock> stockHolder(stock);
    FinancialInstrumentHolder<Option> optionHolder(option);
    FinancialInstrumentHolder<Future> futureHolder(future);

    // Serialize
    std::string serializedStock = stockHolder.Serialize();
    std::string serializedOption = optionHolder.Serialize();
    std::string serializedFuture = futureHolder.Serialize();

    std::cout << "Serialized Stock: " << serializedStock << std::endl;
    std::cout << "Serialized Option: " << serializedOption << std::endl;
    std::cout << "Serialized Future: " << serializedFuture << std::endl;

    // Deserialize
    Stock newStock;
    newStock.Deserialize(serializedStock);
    Option newOption;
    newOption.Deserialize(serializedOption);
    Future newFuture;
    newFuture.Deserialize(serializedFuture);

    std::cout << "Deserialized Stock Symbol: " << newStock.symbol << " Price: " << newStock.price << std::endl;
    std::cout << "Deserialized Option Symbol: " << newOption.symbol << " Strike Price: " << newOption.strikePrice << " Expiry Date: " << newOption.expiryDate << std::endl;
    std::cout << "Deserialized Future Commodity: " << newFuture.commodity << " Contract Size: " << newFuture.contractSize << std::endl;

    return 0;
}