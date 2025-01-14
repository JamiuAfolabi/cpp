#include <iostream>
#include <string>
#include <vector>

bool isMatch(const std::string& text, const std::string& pattern) {
    size_t n = text.size();
    size_t m = pattern.size();

    // Base case: If both text and pattern are empty, return true
    if (n == 0 && m == 0) {
        return true;
    }

    // Base case: If the pattern is empty but text is not, return false
    if (m == 0) {
        return false;
    }

    size_t prevMatch = 0;

    for (size_t i = 0; i < n; i++) {
        size_t j = prevMatch;

        while (j > 0 && text[i] != pattern[j]) {
            j = next(pattern, j - 1);
        }

        if (text[i] == pattern[j]) {
            j++;
        }

        if (j == m) {
            // We found a match, return true
            return true;
        }

        prevMatch = j;
    }

    // Pattern not found, return false
    return false;
}

size_t next(const std::string& pattern, size_t j) {
    if (j == 0) {
        return 0;
    }

    size_t prevMatch = j - 1;

    while (prevMatch > 0 && pattern[prevMatch] != pattern[j]) {
        prevMatch = next(pattern, prevMatch - 1);
    }

    if (pattern[prevMatch] == pattern[j]) {
        prevMatch++;
    }

    return prevMatch;
}

void findPatternWithWildcards(const std::string& text, const std::string& pattern) {
    size_t n = text.size();
    size_t m = pattern.size();
    
    for (size_t i = 0; i <= n - m; ++i) {
        if (isMatch(std::string(text.begin() + i, text.begin() + i + m), pattern)) {
            std::cout << "Pattern found at index: " << i << std::endl;
        }
    }
}

int main() {
    std::string text = "abracadabra";
    std::string pattern = "abr?cad*b";
    
    findPatternWithWildcards(text, pattern);
    return 0;
}