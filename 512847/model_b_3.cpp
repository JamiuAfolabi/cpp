#include <iostream>
#include <string>
#include <string_view>
#include <exception>
#include <cstddef>
#include <cstdint>

class EncodingError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Encoding error";
    }
};

namespace encoding {

const char16_t REPLACEMENT_CHAR = 0xFFFD;

// UTF-8 to UTF-16 conversion with error handling
std::u16string utf8_to_utf16(const std::string_view& utf8_str) {
    std::u16string utf16_str;
    utf16_str.reserve(utf8_str.size() * 2);  // Worst case: each UTF-8 byte becomes 2 UTF-16 code units

    std::size_t utf8_index = 0;
    while (utf8_index < utf8_str.size()) {
        uint8_t first_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);

        if (first_byte < 0x80) {
            // Single-byte ASCII
            utf16_str.push_back(static_cast<char16_t>(first_byte));
        } else if (first_byte < 0xC0) {
            // Invalid leading byte
            utf16_str.push_back(REPLACEMENT_CHAR);
            --utf8_index;  // Step back to try reading the invalid byte again
        } else if (first_byte < 0xE0) {
            // Two-byte sequence
            if (utf8_index >= utf8_str.size()) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid sequence, missing second byte
                continue;
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid second byte
                continue;
            }
            char16_t code_point = (static_cast<char16_t>(first_byte & 0x1F) << 6) | static_cast<char16_t>(second_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else if (first_byte < 0xF0) {
            // Three-byte sequence
            if (utf8_index >= utf8_str.size() - 1) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid sequence, missing bytes
                continue;
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid UTF-8 sequence
                continue;
            }
            char16_t code_point =
                (static_cast<char16_t>(first_byte & 0x0F) << 12) |
                (static_cast<char16_t>(second_byte & 0x3F) << 6) |
                static_cast<char16_t>(third_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else {
            // Four-byte sequence
            if (utf8_index >= utf8_str.size() - 2) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid sequence, missing bytes
                continue;
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t fourth_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80 || (fourth_byte & 0xC0) != 0x80) {
                utf16_str.push_back(REPLACEMENT_CHAR);  // Invalid UTF-8 sequence
                continue;
            }
            char16_t code_point =
                (static_cast<char16_t>(first_byte & 0x07) << 18) |
                (static_cast<char16_t>(second_byte & 0x3F) << 12) |
                (static_cast<char16_t>(third_byte & 0x3F) << 6) |
                static_cast<char16_t>(fourth_byte & 0x3F);
            utf16_str.push_back(code_point);
        }
    }

    return utf16_str;
}

// UTF-16 to UTF-8 conversion with error handling
// UTF-16 to UTF-8 conversion with error handling
std::string utf16_to_utf8(const std::u16string_view& utf16_str) {
    std::string utf8_str;
    utf8_str.reserve(utf16_str.size() * 4);

    char16_t high_surrogate = 0;

    for (char16_t code_point : utf16_str) {
        if (code_point >= 0xD800 && code_point <= 0xDBFF) {
            // High surrogate
            if (high_surrogate != 0) {
                // Malformed: Previous code point was already a high surrogate
                utf8_str.push_back(static_cast<char>(0xEF));
                utf8_str.push_back(static_cast<char>(0xBF));
                utf8_str.push_back(static_cast<char>(0xBD));
                high_surrogate = 0;
            } else {
                high_surrogate = code_point;
                continue; // Wait for the low surrogate
            }
        } else if (code_point >= 0xDC00 && code_point <= 0xDFFF) {
            // Low surrogate
            if (high_surrogate == 0) {
                // Malformed: No previous high surrogate
                utf8_str.push_back(static_cast<char>(0xEF));
                utf8_str.push_back(static_cast<char>(0xBF));
                utf8_str.push_back(static_cast<char>(0xBD));
            } else {
                char32_t supplement = (high_surrogate - 0xD800) << 10 | (code_point - 0xDC00) + 0x10000;
                high_surrogate = 0;

                if (supplement <= 0x10FFFF) {
                    // Valid supplementary character
                    utf8_str.push_back(static_cast<char>(0xF0 | (supplement >> 18)));
                    utf8_str.push_back(static_cast<char>(0x80 | ((supplement >> 12) & 0x3F)));
                    utf8_str.push_back(static_cast<char>(0x80 | ((supplement >> 6) & 0x3F)));
                    utf8_str.push_back(static_cast<char>(0x80 | (supplement & 0x3F)));
                } else {
                    // Invalid supplementary character
                    utf8_str.push_back(static_cast<char>(0xEF));
                    utf8_str.push_back(static_cast<char>(0xBF));
                    utf8_str.push_back(static_cast<char>(0xBD));
                }
            }
        } else {
            if (code_point < 0x0080) {
                // Single-byte ASCII
                utf8_str.push_back(static_cast<char>(code_point));
            } else if (code_point < 0x0800) {
                // Two-byte sequence
                utf8_str.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
                utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
            } else {
                // Three-byte sequence
                utf8_str.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
                utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
                utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
            }
        }
    }

    // Handle any remaining high surrogate
    if (high_surrogate != 0) {
        utf8_str.push_back(static_cast<char>(0xEF));
        utf8_str.push_back(static_cast<char>(0xBF));
        utf8_str.push_back(static_cast<char>(0xBD));
    }

    return utf8_str;
}

}  // namespace encoding

class MainClass {
public:
    static void run() {
        try {
            // Example with invalid UTF-8 character (0xC3 0x28 - invalid because '28' is not a valid continuation byte)
            std::string utf8_str = "Hello, world! \xC3\x28";
            std::u16string utf16_str = encoding::utf8_to_utf16(utf8_str);
            std::string utf8_back = encoding::utf16_to_utf8(utf16_str);

            std::cout << "Original UTF-8: " << utf8_str << std::endl;
            std::cout << "Converted to UTF-16 and back to UTF-8: " << utf8_back << std::endl;

        } catch (const EncodingError& e) {
            std::cerr << "Encoding error: " << e.what() << std::endl;
        }
        try {
    std::u16string utf16_str1 = u"Hello, world!\uD800";  // High surrogate without low surrogate
    std::string utf8_str1 = encoding::utf16_to_utf8(utf16_str1);
    std::cout << "Example 1: \nOriginal UTF-16: " << utf16_str1 << std::endl;
    std::cout << "Converted to UTF-8: " << utf8_str1 << std::endl;

    std::u16string utf16_str2 = u"\uDC00 world!";  // Low surrogate without high surrogate
    std::string utf8_str2 = encoding::utf16_to_utf8(utf16_str2);
    std::cout << "\nExample 2: \nOriginal UTF-16: " << utf16_str2 << std::endl;
    std::cout << "Converted to UTF-8: " << utf8_str2 << std::endl;

    std::u16string utf16_str3 = u"Hello, \uD800\uDC01\uD800 world!";  // Two high surrogates, one low surrogate
    std::string utf8_str3 = encoding::utf16_to_utf8(utf16_str3);
    std::cout << "\nExample 3: \nOriginal UTF-16: " << utf16_str3 << std::endl;
    std::cout << "Converted to UTF-8: " << utf8_str3 << std::endl;

} catch (const EncodingError& e) {
    std::cerr << "Encoding error: " << e.what() << std::endl;
}
    }
    

};

int main() {
    MainClass::run();
    return 0;
}