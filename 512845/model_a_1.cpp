#include <iostream>
#include <string>
#include <vector>

void findAllOccurrences(const std::string &text, const std::string &pattern) {
    int n = text.size();
    int m = pattern.size();
    
    if (m > n) {
        std::cout << "Pattern cannot be longer than the text." << std::endl;
        return;
    }

    for (int i = 0; i <= n - m; i++) {
        // Using sliding window, compare the substring of text starting at i 
        // with the pattern
        bool match = true;
        for (int j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break; // the current window doesn't match
            }
        }

        if (match) {
            std::cout << "Pattern found at index: " << i << std::endl;
        }
    }
}

int main() {
    std::string text = "ababcabcabababd";
    std::string pattern = "abab";
    
    findAllOccurrences(text, pattern);
    
    return 0;
}