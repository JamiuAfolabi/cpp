#include <iostream>
#include <string>
#include <string_view>
#include <exception>
#include <cstddef>
#include <cstdint>

class EncodingError : public std::exception {
public:
    EncodingError(const std::string& message, const std::string& offending_character, size_t position)
        : message_(message), offending_character_(offending_character), position_(position) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    std::string offending_character() const {
        return offending_character_;
    }

    size_t position() const {
        return position_;
    }

private:
    std::string message_;
    std::string offending_character_;
    size_t position_;
};

namespace encoding {

// UTF-8 to UTF-16 conversion with detailed error reporting
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
            throw EncodingError("Invalid UTF-8 leading byte", std::to_string(first_byte), utf8_index - 1);
        } else if (first_byte < 0xE0) {
            // Two-byte sequence
            if (utf8_index >= utf8_str.size()) {
                throw EncodingError("Incomplete UTF-8 sequence", "N/A", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid UTF-8 second byte", std::to_string(second_byte), utf8_index - 1);
            }
            char16_t code_point = (static_cast<char16_t>(first_byte & 0x1F) << 6) | static_cast<char16_t>(second_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else if (first_byte < 0xF0) {
            // Three-byte sequence
            if (utf8_index >= utf8_str.size() - 1) {
                throw EncodingError("Incomplete UTF-8 sequence", "N/A", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid UTF-8 continuation byte", std::to_string(second_byte) + ", " + std::to_string(third_byte), utf8_index - 3);
            }
            char16_t code_point =
                (static_cast<char16_t>(first_byte & 0x0F) << 12) |
                (static_cast<char16_t>(second_byte & 0x3F) << 6) |
                static_cast<char16_t>(third_byte & 0x3F);
            utf16_str.push_back(code_point);
        } else {
            // Four-byte sequence
            if (utf8_index >= utf8_str.size() - 2) {
                throw EncodingError("Incomplete UTF-8 sequence", "N/A", utf8_index - 1);
            }
            uint8_t second_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t third_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            uint8_t fourth_byte = static_cast<uint8_t>(utf8_str[utf8_index++]);
            if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80 || (fourth_byte & 0xC0) != 0x80) {
                throw EncodingError("Invalid UTF-8 continuation byte", std::to_string(second_byte) + ", " + std::to_string(third_byte) + ", " + std::to_string(fourth_byte), utf8_index - 4);
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

// UTF-16 to UTF-8 conversion with detailed error reporting
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
                throw EncodingError("Invalid low surrogate", std::to_string(code_point), it - utf16_str.begin());
            } else if (code_point <= 0xDBFF) {
                // High surrogate, check for valid low surrogate
                if (it + 1 != utf16_str.end()) {
                    char16_t next_code_point = *(it + 1);
                    if (next_code_point >= 0xDC00 && next_code_point <= 0xDFFF) {
                        // Valid surrogate pair
                        uint32_t full_code_point = ((code_point - 0xD800) << 10) + (next_code_point - 0xDC00) + 0x10000;
                        utf8_str.push_back(static_cast<char>(0xF0 | (full_code_point >> 18)));
                        utf8_str.push_back(static_cast<char>(0x80 | ((full_code_point >> 12) & 0x3F)));
                        utf8_str.push_back(static_cast<char>(0x80 | ((full_code_point >> 6) & 0x3F)));
                        utf8_str.push_back(static_cast<char>(0x80 | (full_code_point & 0x3F)));
                        ++it;  // Skip next low surrogate
                    } else {
                        // Invalid low surrogate
                        throw EncodingError("Invalid low surrogate", std::to_string(next_code_point), (it - utf16_str.begin()) + 1);
                    }
                } else {
                    // High surrogate with no following low surrogate
                    throw EncodingError("High surrogate without low surrogate", std::to_string(code_point), it - utf16_str.begin());
                }
            }
            ++it;  // Move to the next character
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
            std::string utf8_str = "Hello, world! \xC3\x28";
            std::u16string utf16_str = encoding::utf8_to_utf16(utf8_str);
            std::string utf8_back = encoding::utf16_to_utf8(utf16_str);

            std::cout << "Original UTF-8: " << utf8_str << std::endl;
            std::cout << "Converted to UTF-16 and back to UTF-8: " << utf8_back << std::endl;

        } catch (const EncodingError& e) {
            std::cerr << "Encoding error: " << e.what() << "\n"
                      << "Offending character/byte: " << e.offending_character() << "\n"
                      << "Error position: " << e.position() << std::endl;
        }
    }
};

int main() {
    MainClass::run();
    return 0;
}
