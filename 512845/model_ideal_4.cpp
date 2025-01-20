#include <iostream>
#include <regex>

int main() {
    std::string text = "hello world";
    std::string pattern1 = "he.*d";  
    std::string pattern2 = ".*world"; 
    std::string pattern3 = "h.*l";  
    std::string pattern4 = ".*o.*"; 

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
