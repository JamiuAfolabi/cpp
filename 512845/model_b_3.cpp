#include <iostream>
#include <string>
#include <regex>

bool doesPatternMatch(const std::string& text, const std::string& pattern) {
    // Replace '*' with '.*' and '?' with '.' in the pattern
    std::string regexPattern = pattern.replace(pattern.begin(), pattern.end(), '*', ".*").replace(pattern.begin(), pattern.end(), '?', ".");

    // Compile the regular expression pattern
    std::regex re(regexPattern);

    // Use the smatch data structure to store the matches
    std::smatch match;

    return std::regex_match(text, match, re);
}

int main() {
    std::string text = "abcdef";
    std::string pattern = "a?d*f";

    if (doesPatternMatch(text, pattern)) {
        std::cout << "Match found." << std::endl;
    } else {
        std::cout << "No match found." << std::endl;
    }

    return 0;
}