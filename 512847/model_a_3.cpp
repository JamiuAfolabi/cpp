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
// UTF-16 to UTF-8 conversion with enhanced error handling
std::string utf16_to_utf8(const std::u16string_view& utf16_str) {
    std::string utf8_str;
    utf8_str.reserve(utf16_str.size() * 4);  // Worst case: each UTF-16 code unit becomes 4 UTF-8 bytes

    for (std::size_t i = 0; i < utf16_str.size(); ++i) {
        char16_t code_point = utf16_str[i];

        if (code_point < 0x0080) {
            // Single-byte ASCII
            utf8_str.push_back(static_cast<char>(code_point));
        } else if (code_point < 0x0800) {
            // Two-byte sequence
            utf8_str.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else if (code_point >= 0xD800 && code_point <= 0xDBFF) {
            // High surrogate
            if (i + 1 < utf16_str.size()) {
                char16_t low_surrogate = utf16_str[i + 1];
                if (low_surrogate >= 0xDC00 && low_surrogate <= 0xDFFF) {
                    // Valid surrogate pair
                    uint32_t actual_code_point = ((static_cast<uint32_t>(code_point) - 0xD800) << 10)
                        + (static_cast<uint32_t>(low_surrogate) - 0xDC00) + 0x10000;
                    utf8_str.push_back(static_cast<char>(0xF0 | (actual_code_point >> 18)));
                    utf8_str.push_back(static_cast<char>(0x80 | ((actual_code_point >> 12) & 0x3F)));
                    utf8_str.push_back(static_cast<char>(0x80 | ((actual_code_point >> 6) & 0x3F)));
                    utf8_str.push_back(static_cast<char>(0x80 | (actual_code_point & 0x3F)));
                    ++i; // Advance to the next code unit
                } else {
                    // Invalid low surrogate, replace with REPLACEMENT_CHAR
                    utf8_str.push_back(static_cast<char>(0xEF));
                    utf8_str.push_back(static_cast<char>(0xBF));
                    utf8_str.push_back(static_cast<char>(0xBD));
                }
            } else {
                // High surrogate without a low surrogate
                utf8_str.push_back(static_cast<char>(0xEF));
                utf8_str.push_back(static_cast<char>(0xBF));
                utf8_str.push_back(static_cast<char>(0xBD));
            }
        } else if (code_point >= 0xDC00 && code_point <= 0xDFFF) {
            // Invalid low surrogate alone
            utf8_str.push_back(static_cast<char>(0xEF));
            utf8_str.push_back(static_cast<char>(0xBF));
            utf8_str.push_back(static_cast<char>(0xBD));
        } else {
            // Three-byte sequence
            utf8_str.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
            utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        }
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
            std::u16string utf16_str = u"\xD800"; // Invalid surrogate pair (high surrogate without low surrogate)
            std::string utf8_back = encoding::utf16_to_utf8(utf16_str);

            std::cout << "UTF-16 with invalid surrogate pair: " << utf16_str << std::endl;
            std::cout << "Converted to UTF-8: " << utf8_back << std::endl;
        } catch (const EncodingError& e) {
            std::cerr << "Encoding error: " << e.what() << std::endl;
        }

    }
};

int main() {
    MainClass::run();
    return 0;
}