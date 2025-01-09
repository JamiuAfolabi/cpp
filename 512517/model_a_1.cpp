#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <unordered_map>
#include <variant>
#include <concepts>
#include <locale>

template<typename T>
concept LocaleIdentifier = std::same_as<T, std::string>;

template<typename T>
concept CurrencyCode = std::same_as<T, std::string>;

template<typename T>
concept DateTimeFormatter = std::same_as<T, std::string>;

template<typename Locale, LocaleIdentifier LID, CurrencyCode CCode, DateTimeFormatter DFormatter>
class Internationalization {
public:
    Internationalization(LID localeId, CCode currencyCode, DFormatter dateTimeFormat)
        : localeId(localeId), currencyCode(currencyCode), dateTimeFormat(dateTimeFormat) {}

    std::string formatDate(const std::chrono::system_clock::time_point& timePoint) const {
        auto timeT = std::chrono::system_clock::to_time_t(timePoint);
        std::tm tm = {};
        localtime_s(&tm, &timeT);
        return std::format(dateTimeFormat, std::put_time(&tm, dateTimeFormat.c_str()));
    }

    std::string formatCurrency(double amount) const {
        return std::format("{} {:.2f}", currencyCode, amount);
    }

    std::string getLocale() const {
        return localeId;
    }

private:
    LID localeId;
    CCode currencyCode;
    DFormatter dateTimeFormat;
};

// Example of extending for a new locale
template<typename T>
class LocaleProvider {
public:
    LocaleProvider() {
        // Initialize with some default locales, e.g., English and French
        locales["en-US"] = Internationalization<std::string, "en-US", "USD", "%Y-%m-%dT%H:%M:%S"};
        locales["fr-FR"] = Internationalization<std::string, "fr-FR", "EUR", "%d-%m-%Y %H:%M:%S"};
    }

    Internationalization<std::string, std::string, std::string, std::string>& getLocale(const std::string& id) {
        return locales.at(id);
    }

    void addLocale(const std::string& id, Internationalization<std::string, std::string, std::string, std::string> locale) {
        locales[id] = locale;
    }

private:
    std::unordered_map<std::string, Internationalization<std::string, std::string, std::string, std::string>> locales;
};

int main() {
    LocaleProvider<std::string> localeProvider;

    auto locale = localeProvider.getLocale("en-US");
    std::cout << "Locale: " << locale.getLocale() << "\n";
    std::cout << "Date: " << locale.formatDate(std::chrono::system_clock::now()) << "\n";
    std::cout << "Currency: " << locale.formatCurrency(1234.56) << "\n";

    return 0;
}