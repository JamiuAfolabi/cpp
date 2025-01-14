#include <iostream>
#include <string>

using namespace std;

int stringMatch(string text, string pattern) {
    // Base case: If the pattern is empty, return 0
    if (pattern.empty()) {
        return 0;
    }

    // Base case: If the text is empty, return -1
    if (text.empty()) {
        return -1;
    }

    int text_size = text.size();
    int pattern_size = pattern.size();

    for (int i = 0; i <= text_size - pattern_size; i++) {
        int j;
        // Initialize a sliding window starting at index 'i'
        for (j = 0; j < pattern_size; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }

        // If we reach here, it means we have found a match
        if (j == pattern_size) {
            return i;
        }
    }

    // If no match is found, return -1
    return -1;
}

int main() {
    string text, pattern;

    cout << "Enter the text: ";
    getline(cin, text);

    cout << "Enter the pattern: ";
    getline(cin, pattern);

    int index = stringMatch(text, pattern);

    if (index == -1) {
        cout << "Pattern not found." << endl;
    } else {
        cout << "Pattern found at index " << index << endl;
    }

    return 0;
}
