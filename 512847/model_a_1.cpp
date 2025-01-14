#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <stdexcept>

class I18N {
public:
    // Convert UTF-8 string to UTF-16
    static std::wstring utf8_to_utf16(const std::string& utf8) {
        // Create a UTF-8 to UTF-16 codecvt facet
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(utf8);
    }
    
    // Convert UTF-16 string to UTF-8
    static std::string utf16_to_utf8(const std::wstring& utf16) {
        // Create a UTF-16 to UTF-8 codecvt facet
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(utf16);
    }
    
    // Function to print UTF-8 string
    static void print_utf8(const std::string& str) {
        std::cout << "UTF-8: " << str << std::endl;
    }

    // Function to print UTF-16 string
    static void print_utf16(const std::wstring& str) {
        std::wcout << L"UTF-16: " << str << std::endl;
    }

    // Example of string operations
    static void example_operations() {
        std::string utf8_string = u8"Hello, 世界!";
        std::wstring utf16_string = utf8_to_utf16(utf8_string);

        print_utf8(utf8_string);
        print_utf16(utf16_string);

        // Convert back and forth
        std::string converted_back = utf16_to_utf8(utf16_string);
        print_utf8(converted_back);
    }
};

int main() {
    try {
        I18N::example_operations();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}