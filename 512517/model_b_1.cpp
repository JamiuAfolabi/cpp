#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <exception>

#include <concepts>

template <typename T>
concept StringType = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

namespace i18n {
    struct InvalidLocaleError : std::runtime_error {
        InvalidLocaleError(const std::string& message) : std::runtime_error("Invalid locale: " + message) {}
    };
    
    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        // Function to format the date and time according to ISO 8601
        template<StringType T>
        T to_iso8601() const {
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(4) << year << '-' << std::setfill('0') << std::setw(2) << month << '-' << std::setfill('0') << std::setw(2) << day;
            oss << 'T' << std::setfill('0') << std::setw(2) << hour << ':' << std::setfill('0') << std::setw(2) << minute << ':' << std::setfill('0') << std::setw(2) << second;
            return T(oss.str());
        }
    };

    // Simple mapping for ISO 4217 currency codes to their corresponding symbols
    inline const std::map<std::string, std::string> iso4217_to_symbol = {
        {"USD", "$"}, {"EUR", "€"}, {"GBP", "£"}, {"JPY", "¥"}, {"CNY", "¥"}
    };

    // Lookup currency symbol from ISO 4217 code
    template <StringType T>
    T get_currency_symbol(T const& iso4217_code) {
        auto it = iso4217_to_symbol.find(iso4217_code.to_string());
        if (it == iso4217_to_symbol.end()) {
            throw InvalidLocaleError("Unrecognized currency code");
        }
        return it->second;
    }

    // Locale information storage
    struct LocaleInfo {
        std::string language_tag;
        std::string date_format;
        std::string currency_code;
    };

    // Load locale data from some data source (e.g., a database, file, or built-in data)
    LocaleInfo load_locale(std::string const& locale_id) {
        if (locale_id == "en_US") {
            return {"en_US", "MM/dd/yyyy", "USD"};
        } else if (locale_id == "fr_FR") {
            return {"fr_FR", "dd/MM/yyyy", "EUR"};
        } else {
            throw InvalidLocaleError("Unsupported locale");
        }
    }

    class Formatter {
    private:
        LocaleInfo locale;

    public:
        explicit Formatter(std::string const& locale_id) : locale(load_locale(locale_id)) {}

        // Format date and time using the loaded locale
        template<StringType T>
        T format_date(const DateTime& dt) const {
            // Implement the actual date formatting based on the locale's date_format string
            std::ostringstream oss;
            // Extract the date components from DateTime
            oss << dt.day << '/' << dt.month << '/' << dt.year;
            return T(oss.str());
        }

        // Format currency using the loaded locale
        template<StringType T>
        T format_currency(double amount) const {
            return get_currency_symbol(locale.currency_code) + std::to_string(amount);
        }
    };
}

int main() {
    try {
        i18n::DateTime dt {2023, 7, 31, 12, 0, 0};

        i18n::Formatter us_formatter("en_US");
        auto us_date = us_formatter.format_date(dt);
        auto us_currency = us_formatter.format_currency(199.99);
        std::cout << "US Date: " << us_date << std::endl;  // Output: US Date: 31/07/2023
        std::cout << "US Currency: " << us_currency << std::endl;  // Output: US Currency: $199.99

        i18n::Formatter fr_formatter("fr_FR");
        auto fr_date = fr_formatter.format_date(dt);
        auto fr_currency = fr_formatter.format_currency(89.95);
        std::cout << "FR Date: " << fr_date << std::endl;  // Output: FR Date: 31/07/2023
        std::cout << "FR Currency: " << fr_currency << std::endl;  // Output: FR Currency: €89.95
    } catch (const i18n::InvalidLocaleError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}