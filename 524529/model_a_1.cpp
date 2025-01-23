#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <iomanip>

class Serializable {
public:
    virtual std::string Serialize() const = 0;
    virtual void Deserialize(const std::string& data) = 0;
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

    std::string Serialize() const override {
        std::ostringstream oss;
        oss << "Stock:" << symbol << "," << price;
        return oss.str();
    }

    void Deserialize(const std::string& data) override {
        size_t pos = data.find(':');
        if (pos == std::string::npos) return;
        std::string info = data.substr(pos + 1);
        pos = info.find(',');
        if (pos == std::string::npos) return;
        symbol = info.substr(0, pos);
        price = std::stod(info.substr(pos + 1));
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
        std::ostringstream oss;
        oss << "Option:" << symbol << "," << strikePrice << "," << expiryDate;
        return oss.str();
    }

    void Deserialize(const std::string& data) override {
        size_t pos = data.find(':');
        if (pos == std::string::npos) return;
        std::string info = data.substr(pos + 1);
        pos = info.find(',');
        if (pos == std::string::npos) return;
        symbol = info.substr(0, pos);
        size_t nextPos = info.find(',', pos + 1);
        if (nextPos == std::string::npos) return;
        strikePrice = std::stod(info.substr(pos + 1, nextPos - pos - 1));
        expiryDate = info.substr(nextPos + 1);
    }
};

class Future : public FinancialInstrument {
public:
    std::string commodity;
    double contractSize;

    Future() = default;
    Future(const std::string& commodity, double contractSize) : commodity(commodity), contractSize(contractSize) {}

    std::string Serialize() const override {
        std::ostringstream oss;
        oss << "Future:" << commodity << "," << contractSize;
        return oss.str();
    }

    void Deserialize(const std::string& data) override {
        size_t pos = data.find(':');
        if (pos == std::string::npos) return;
        std::string info = data.substr(pos + 1);
        pos = info.find(',');
        if (pos == std::string::npos) return;
        commodity = info.substr(0, pos);
        contractSize = std::stod(info.substr(pos + 1));
    }
};

// Templated holder for any financial instrument
template<typename T>
class FinancialInstrumentHolder {
private:
    T instrument;

public:
    FinancialInstrumentHolder(const T& instrument) : instrument(instrument) {}

    std::string Serialize() const {
        return instrument.Serialize();
    }

    void Deserialize(const std::string& data) {
        instrument.Deserialize(data);
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