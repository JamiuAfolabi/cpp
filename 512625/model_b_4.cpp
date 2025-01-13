#include <iostream>
#include <unordered_set>
#include <set>
#include <type_traits>
#include <string>

using namespace std;

// Template alias to choose between unordered_set and set
template <bool UseUnordered>
using MatchSet = typename std::conditional<
    UseUnordered,
    unordered_set<int>,
    set<int>
>::type;

// Function to compute prefix shift array
void computePrefixShift(int shiftArray[], int suffixArray[], const string& pattern) {
    const int patternLength = static_cast<int>(pattern.size()); 
    int i = patternLength;
    int j = patternLength + 1;
    suffixArray[i] = j;

    while (i > 0) {
        while (j <= patternLength && pattern[i - 1] != pattern[j - 1]) {
            if (shiftArray[j] == 0) {
                shiftArray[j] = j - i; 
            }
            j = suffixArray[j]; 
        }
        i--;
        j--;
        suffixArray[i] = j; 
    }
}

// Function to compute suffix shift array
void computeSuffixShift(int shiftArray[], int suffixArray[], const string& pattern) {
    const int patternLength = static_cast<int>(pattern.size());
    int j = suffixArray[0];

    for (int i = 0; i < patternLength; i++) {
        if (shiftArray[i] == 0) {
            shiftArray[i] = j; 
            if (i == j) {
                j = suffixArray[j]; 
            }
        }
    }
}

// Function to find all pattern matches in the text
template <bool UseUnordered>
void findPatternMatches(const string& text, const string& pattern, MatchSet<UseUnordered>& matchSet) {
    const int patternLength = static_cast<int>(pattern.size()); 
    const int textLength = static_cast<int>(text.size());
    int suffixArray[patternLength + 1];
    int shiftArray[patternLength + 1];

    for (int i = 0; i <= patternLength; i++) {
        shiftArray[i] = 0;
    }

    computePrefixShift(shiftArray, suffixArray, pattern); 
    computeSuffixShift(shiftArray, suffixArray, pattern); 

    int shift = 0;
    size_t expectedMatches = (textLength - patternLength) / patternLength; 

    // Reserve memory in advance
    if constexpr (UseUnordered) {
        matchSet.reserve(expectedMatches + 10); // Adding some extra capacity
    } else {
        matchSet.reserve(expectedMatches + 10); // Adding some extra capacity
    }

    while (shift <= (textLength - patternLength)) {
        int j = patternLength - 1;
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        if (j < 0) {
            matchSet.insert(shift);
            shift += shiftArray[0];
        } else {
            shift += shiftArray[j + 1];
        }
    }
}

int main() {
    string text, pattern;
    bool useUnordered;

    cout << "Would you like to use unordered_set instead of set? (1 for yes, 0 for no): ";
    cin >> useUnordered;

    cout << "Enter the original string: ";
    cin >> text;
    cout << "Enter the pattern to search: ";
    cin >> pattern;
