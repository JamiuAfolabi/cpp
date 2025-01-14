#ifndef STRING_ENCODING_H
#define STRING_ENCODING_H

#include <string>
#include <stdexcept>
#include <unicode/unistr.h> // ICU4C Unicode String

class StringEncoding {
public:
    // Converts a UTF-8 string to UTF-16
    static std::u16string utf8ToUtf16(const std::string& utf8Str) {
        // Use ICU to convert UTF-8 to UTF-16
        icu::UnicodeString unicodeStr = icu::UnicodeString::fromUTF8(utf8Str);
        return std::u16string(unicodeStr.getBuffer(), unicodeStr.length());
    }

    // Converts a UTF-16 string to UTF-8
    static std::string utf16ToUtf8(const std::u16string& utf16Str) {
        icu::UnicodeString unicodeStr(reinterpret_cast<const UChar *>(utf16Str.data()), static_cast<int32_t>(utf16Str.length()));
        std::string utf8Str;
        unicodeStr.toUTF8String(utf8Str);
        return utf8Str;
    }

    // Gets the length of a UTF-8 string in terms of UTF-16 code units
    static size_t utf8LengthInUtf16(const std::string& utf8Str) {
        icu::UnicodeString unicodeStr = icu::UnicodeString::fromUTF8(utf8Str);
        return unicodeStr.length();
    }

    // Gets the length of a UTF-16 string
    static size_t utf16Length(const std::u16string& utf16Str) {
        return utf16Str.length();
    }
};

#endif // STRING_ENCODING_H