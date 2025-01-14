#include <iostream>
#include <string>
using namespace std;

int matchPatternRecursively(string text, string& pattern, int textLen, int patternLen) {
    if (patternLen == 0) return (textLen == 0) ? 0 : -1;
    if (textLen == 0) {
        for (int i = 0; i < patternLen; i++) if (pattern[i] != '*') return -1;
        return 0;
    }

    if (text[textLen - 1] == pattern[patternLen - 1] || pattern[patternLen - 1] == '?')
        return matchPatternRecursively(text, pattern, textLen - 1, patternLen - 1);

    if (pattern[patternLen - 1] == '*') {
        int result = matchPatternRecursively(text, pattern, textLen, patternLen - 1);
        if (result != -1) return result;
        return matchPatternRecursively(text, pattern, textLen - 1, patternLen);
    }

    return -1;
}

int doesPatternMatch(string text, string pattern) {
    int textLen = text.size(), patternLen = pattern.size();

    if (pattern[0] != '*') {
        for (int i = 0; i < textLen; ++i) {
            if (text[i] == pattern[0]) {
                int result = matchPatternRecursively(text.substr(i), pattern, textLen - i, patternLen);
                if (result != -1) return i; 
            }
        }
    } else {
        return matchPatternRecursively(text, pattern, textLen, patternLen);
    }

    return -1;
}

int main() {
    string text = "vvabcde";
    string pattern = "a*d?";
    int result = doesPatternMatch(text, pattern);
    if (result != -1) {
        cout << "Match found at index " << result;
    } else {
        cout << "No match found";
    }
    return 0;
}
