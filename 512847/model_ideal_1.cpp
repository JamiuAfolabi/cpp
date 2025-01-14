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

// UTF-8 to UTF-16 conversion
std::u16string utf8_to_utf16(const std::string_view& utf8_str) {
    std::u16string utf16_str;
    utf16_str.reserve(utf8_str.size() * 2);  // Worst case: each UTF-8 byte becomes 2 UTF-16 code units

    std::size_t utf8_index = 0;
    for (; utf8_index < utf8_str.size();) {
        uint8_t first_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);

        if (first_byte < 0x80) {
            // Single-byte ASCII
            utf16_str.push_back(static_cast<char16_t>(first_byte));
        } else if (first_byte < 0xC0) {
            throw EncodingError();  // Invalid UTF-8 sequence
        } else if (first_byte < 0xE0) {
            // Two-byte sequence
            if (utf8_index >= utf8_str.size()) {
                throw EncodingError();  // Invalid UTF-8 sequence: missing second byte
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80) {
                throw EncodingError();  // Invalid UTF-8 sequence: wrong second byte
            }
            char16_t code_point = (static_cast<char16_t>(first_byte & 0x1F) << 6) | static_cast<char16_t>(second_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else if (first_byte < 0xF0) {
            // Three-byte sequence
            if (utf8_index >= utf8_str.size() - 1) {
                throw EncodingError();  // Invalid UTF-8 sequence: missing second or third byte
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80) {
                throw EncodingError();  // Invalid UTF-8 sequence: wrong second or third byte
            }
            char16_t code_point =
                (static_cast<char16_t>(first_byte & 0x0F) << 12) |
                (static_cast<char16_t>(second_byte & 0x3F) << 6) |
                static_cast<char16_t>(third_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else {
            // Four-byte sequence
            if (utf8_index >= utf8_str.size() - 2) {
                throw EncodingError();  // Invalid UTF-8 sequence: missing second, third, or fourth byte
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t fourth_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80 || (fourth_byte & 0xC0) != 0x80) {
                throw EncodingError();  // Invalid UTF-8 sequence: wrong second, third, or fourth byte
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

// UTF-16 to UTF-8 conversion
std::string utf16_to_utf8(const std::u16string_view& utf16_str) {
    std::string utf8_str;
    utf8_str.reserve(utf16_str.size() * 4);  // Worst case: each UTF-16 code unit becomes 4 UTF-8 bytes

    for (char16_t code_point : utf16_str) {
        if (code_point < 0x0080) {
            // Single-byte ASCII
            utf8_str.push_back(static_cast<char>(code_point));
        } else if (code_point < 0x0800) {
            // Two-byte sequence
            utf8_str.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else if (code_point < 0xD800 || code_point >= 0xE000) {
            // Three-byte sequence
            utf8_str.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
            utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
        } else {
            // Surrogate pair: invalid in UTF-16
            throw EncodingError();
        }
    }

    return utf8_str;
}

}  // namespace encoding

class MainClass {
public:
    static void run() {
        try {
            std::string utf8_str = "Hello, world!";
            std::u16string utf16_str = encoding::utf8_to_utf16(utf8_str);
            std::string utf8_back = encoding::utf16_to_utf8(utf16_str);

            std::cout << "Original UTF-8: " << utf8_str << std::endl;
            std::cout << "Converted to UTF-16 and back to UTF-8: " << utf8_back << std::endl;
        } catch (const EncodingError& e) {
            std::cerr << "Encoding error: " << e.what() << std::endl;
        }
    }
};

int main() {
    MainClass::run();
    return 0;
}
