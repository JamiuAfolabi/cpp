#include <iostream>
#include <string>
#include <string_view>
#include <exception>
#include <cstddef>
#include <cstdint>

class EncodingError : public std::exception {
public:
    EncodingError(const std::string& message, size_t position)
        : msg(message), pos(position) {}

    const char* what() const noexcept override {
        return ("Encoding error at position " + std::to_string(pos) + ": " + msg).c_str();
    }

    size_t get_position() const {
        return pos;
    }

    std::string get_message() const {
        return msg;
    }

private:
    std::string msg;
    size_t pos;
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
            throw EncodingError("Invalid leading byte", utf8_index - 1);
        } else if (first_byte < 0xE0) {
            // Two-byte sequence
            if (utf8_index >= utf8_str.size()) {
                throw EncodingError("Invalid sequence: missing second byte", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid second byte", utf8_index - 1);
            }
            char16_t code_point = (static_cast<char16_t>(first_byte & 0x1F) << 6) | static_cast<char16_t>(second_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else if (first_byte < 0xF0) {
            // Three-byte sequence
            if (utf8_index >= utf8_str.size() - 1) {
                throw EncodingError("Invalid sequence: missing bytes", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid UTF-8 sequence", utf8_index - 1);
            }
            char16_t code_point =
                (static_cast<char16_t>(first_byte & 0x0F) << 12) |
                (static_cast<char16_t>(second_byte & 0x3F) << 6) |
                static_cast<char16_t>(third_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else {
            // Four-byte sequence
            if (utf8_index >= utf8_str.size() - 2) {
                throw EncodingError("Invalid sequence: missing bytes", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t fourth_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80 || (fourth_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid UTF-8 sequence", utf8_index - 1);
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
std::string utf16_to_utf8(const std::u16string_view& utf16_str) {
    std::string utf8_str;
    utf8_str.reserve(utf16_str.size() * 4);  // Worst case: each UTF-16 code unit becomes 4 UTF-8 bytes

    auto it = utf16_str.begin();
    while (it != utf16_str.end()) {
        char16_t code_point = *it;

        if (code_point < 0x0080) {
            // Single-byte ASCII
            utf8_str.push_back(static_cast<char>(code_point));
            ++it;
        } else if (code_point < 0x0800) {
            // Two-byte sequence
            utf8_str.push_back(static_cast<char>(0xC0 | (code_point >> 6)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
            ++it;
        } else if (code_point >= 0xD800 && code_point <= 0xDFFF) {
            // Invalid UTF-16 surrogate pair
            if (code_point >= 0xDC00 && code_point <= 0xDFFF) {
                // A low surrogate without a preceding high surrogate
                throw EncodingError("Invalid surrogate pair: low surrogate without high surrogate", std::distance(utf16_str.begin(), it));
            } else if (code_point <= 0xDBFF) {
                // High surrogate, check for valid low surrogate
                if (it + 1 != utf16_str.end()) {
                    char16_t next_code_point = *(it + 1);
                    if (next_code_point >= 0xDC00 && next_code_point <= 0xDFFF) {
                        // Valid surrogate pair
                        uint32_t code_point = ((code_point - 0xD800) << 10) + (next_code_point - 0xDC00) + 0x10000;
                        utf8_str.push_back(static_cast<char>(0xF0 | (code_point >> 18)));
                        utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 12) & 0x3F)));
                        utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
                        utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
                        ++it;  // Skip next low surrogate
                    } else {
                        // Invalid low surrogate, replace with replacement character
                        throw EncodingError("Invalid surrogate pair: invalid low surrogate", std::distance(utf16_str.begin(), it));
                    }
                } else {
                    // High surrogate with no following low surrogate
                    throw EncodingError("Invalid surrogate pair: high surrogate without low surrogate", std::distance(utf16_str.begin(), it));
                }
            }
            ++it;
        } else {
            // Three-byte sequence
            utf8_str.push_back(static_cast<char>(0xE0 | (code_point >> 12)));
            utf8_str.push_back(static_cast<char>(0x80 | ((code_point >> 6) & 0x3F)));
            utf8_str.push_back(static_cast<char>(0x80 | (code_point & 0x3F)));
            ++it;
        }
    }

    return utf8_str;
}

}  // namespace encoding

class MainClass {
public:
    static void run() {
        try {
            // Test with invalid UTF-8 character (0xC3 0x28 - invalid because '28' is not a valid continuation byte)
            std::string utf8_str = "Hello, world! \xC3\x28";
            std::u16string utf16_str = encoding::utf8_to_utf16(utf8_str);
            std::string utf8_back = encoding::utf16_to_utf8(utf16_str);

            std::cout << "Original UTF-8: " << utf8_str << std::endl;
            std::cout << "Converted to UTF-16 and back to UTF-8: " << utf8_back << std::endl;

            // Test with invalid surrogate pair (high surrogate without low surrogate)
            std::u16string invalid_utf16 = u"\xD800";  // High surrogate without low surrogate
            std::string invalid_utf8 = encoding::utf16_to_utf8(invalid_utf16);
            std::cout << "Converted invalid surrogate pair (high surrogate only) to UTF-8: " << invalid_utf8 << std::endl;

            // Test with valid surrogate pair (high surrogate and low surrogate)
            std::u16string valid_utf16 = u"\xD800\xDC00";  // Valid surrogate pair
            std::string valid_utf8 = encoding::utf16_to_utf8(valid_utf16);
            std::cout << "Converted valid surrogate pair to UTF-8: " << valid_utf8 << std::endl;

            // Test with low surrogate without high surrogate
            std::u16string lone_low_surrogate = u"\xDC00";  // Low surrogate without high surrogate
            std::string lone_low_utf8 = encoding::utf16_to_utf8(lone_low_surrogate);
            std::cout << "Converted low surrogate without high surrogate to UTF-8: " << lone_low_utf8 << std::endl;

        } catch (const EncodingError& e) {
            std::cerr << "Encoding error: " << e.what() << std::endl;
        }
    }
};

int main() {
    MainClass::run();
    return 0;
}
