#include <iostream>
#include <regex>

int main() {
    std::string text = "hello world";
    std::string pattern1 = "he.*d";  // This won't match "hello world"
    std::string pattern2 = ".*world";  // This will match but it must match the whole string
    std::string pattern3 = "h.*l";  // This won't match "hello world"
    std::string pattern4 = ".*o.*";  // This will match "hello world"

    if (std::regex_match(text, std::regex(pattern1))) {
        std::cout << "Match found for pattern1: " << pattern1 << std::endl;
    } else {
        std::cout << "No match found for pattern1: " << pattern1 << std::endl;
    }

    if (std::regex_match(text, std::regex(pattern2))) {
        std::cout << "Match found for pattern2: " << pattern2 << std::endl;
    } else {
        std::cout << "No match found for pattern2: " << pattern2 << std::endl;
    }

    if (std::regex_match(text, std::regex(pattern3))) {
        std::cout << "Match found for pattern3: " << pattern3 << std::endl;
    } else {
        std::cout << "No match found for pattern3: " << pattern3 << std::endl;
    }

    if (std::regex_match(text, std::regex(pattern4))) {
        std::cout << "Match found for pattern4: " << pattern4 << std::endl;
    } else {
        std::cout << "No match found for pattern4: " << pattern4 << std::endl;
    }

    return 0;
}
