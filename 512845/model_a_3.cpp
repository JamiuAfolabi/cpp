#include <iostream>
#include <string>
#include <regex>

bool doesPatternMatch(const std::string &text, const std::string &pattern) {
    // Convert wildcard pattern to regex pattern
    std::string regexStr = pattern;
    
    // Replace '?' with '.'
    std::replace(regexStr.begin(), regexStr.end(), '?', '.');
    
    // Replace '*' with '.*'
    size_t pos = 0;
    while ((pos = regexStr.find('*', pos)) != std::string::npos) {
        regexStr.replace(pos, 1, ".*");
        pos += 2; // Move past the new '.*'
    }
    
    // Create regex object from the modified pattern
    std::regex regexPattern("^" + regexStr + "$");

    // Check if the text matches the regex pattern
    return std::regex_match(text, regexPattern);
}

int main() {
    std::string text = "ababcabcabababd";
    std::string pattern = "ab*?d"; // Example pattern with wildcards
    
    if (doesPatternMatch(text, pattern)) {
        std::cout << "Match found!" << std::endl;
    } else {
        std::cout << "No match found." << std::endl;
    }

    return 0;
}