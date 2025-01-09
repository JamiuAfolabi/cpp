#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <exception>
#include <sstream>
#include <iomanip>

template <typename T>
concept StringType = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

namespace i18n {
    struct InvalidLocaleError : std::runtime_error {
        InvalidLocaleError(const std::string& message, const std::string& invalid_value) 
            : std::runtime_error(message + ": " + invalid_value) {}
    };
    
    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        template<StringType T>
        T to_iso8601() const {
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(4) << year << '-' 
                << std::setfill('0') << std::setw(2) << month << '-' 
                << std::setfill('0') << std::setw(2) << day << 'T'
                << std::setfill('0') << std::setw(2) << hour << ':' 
                << std::setfill('0') << std::setw(2) << minute << ':' 
                << std::setfill('0') << std::setw(2) << second;
            return T(oss.str());
        }
    };

    inline const std::map<std::string, std::string> iso4217_to_symbol = {
        {"USD", "$"}, {"EUR", "€"}, {"GBP", "£"}, {"JPY", "¥"}, {"CNY", "¥"}
    };

    template <StringType T>
    T get_currency_symbol(const std::string& iso4217_code) {
        auto it = iso4217_to_symbol.find(iso4217_code);
        if (it == iso4217_to_symbol.end()) {
            throw InvalidLocaleError("Unrecognized currency code", iso4217_code);
        }
        return T(it->second);
    }

    struct LocaleInfo {
        std::string language_tag;
        std::string date_format;
        std::string currency_code;
    };

    LocaleInfo load_locale(const std::string& locale_id) {
        if (locale_id == "en_US") {
            return {"en_US", "MM/dd/yyyy", "USD"};
        } else if (locale_id == "fr_FR") {
            return {"fr_FR", "dd/MM/yyyy", "EUR"};
        } else {
            throw InvalidLocaleError("Unsupported locale", locale_id);
        }
    }

    class Formatter {
    private:
        LocaleInfo locale;

    public:
        explicit Formatter(const std::string& locale_id) : locale(load_locale(locale_id)) {}

        template<StringType T>
        T format_date(const DateTime& dt) const {
            std::ostringstream oss;
            if (locale.date_format == "MM/dd/yyyy") {
                oss << std::setfill('0') << std::setw(2) << dt.month << '/' 
                    << std::setfill('0') << std::setw(2) << dt.day << '/' 
                    << dt.year;
            } else if (locale.date_format == "dd/MM/yyyy") {
                oss << std::setfill('0') << std::setw(2) << dt.day << '/' 
                    << std::setfill('0') << std::setw(2) << dt.month << '/' 
                    << dt.year;
            }
            return T(oss.str());
        }

        template<StringType T>
        T format_currency(double amount) const {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << amount;
            return get_currency_symbol<T>(locale.currency_code) + T(oss.str());
        }
    };
}

int main() {
    try {
        i18n::DateTime dt {2023, 7, 31, 12, 0, 0};

        i18n::Formatter us_formatter("en_US");
        auto us_date = us_formatter.format_date<std::string>(dt);
        auto us_currency = us_formatter.format_currency<std::string>(199.99);
        auto us_iso8601 = dt.to_iso8601<std::string>();  // Print ISO 8601
        std::cout << "US Date: " << us_date << std::endl;
        std::cout << "US Currency: " << us_currency << std::endl;
        std::cout << "US DateTime (ISO 8601): " << us_iso8601 << std::endl;

        // Intentionally trying to load an unsupported locale
        i18n::Formatter fr_formatter("fr_FR");
        auto fr_date = fr_formatter.format_date<std::string>(dt);
        auto fr_currency = fr_formatter.format_currency<std::string>(89.95);
        auto fr_iso8601 = dt.to_iso8601<std::string>();  // Print ISO 8601
        std::cout << "FR Date: " << fr_date << std::endl;
        std::cout << "FR Currency: " << fr_currency << std::endl;
        std::cout << "FR DateTime (ISO 8601): " << fr_iso8601 << std::endl;

        // Attempt to get currency symbol for an unrecognized code
        auto invalid_currency = i18n::get_currency_symbol<std::string>("XYZ");

    } catch (const i18n::InvalidLocaleError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}