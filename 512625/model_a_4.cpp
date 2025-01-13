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
void computePrefixShift(int shiftArray[], int suffixArray[], string pattern) {
    int patternLength = pattern.size(); 
    int i = patternLength;
    int j = patternLength + 1;
    suffixArray[i] = j;

    while(i > 0) {
        while(j <= patternLength && pattern[i-1] != pattern[j-1]) {
            if(shiftArray[j] == 0) {
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
void computeSuffixShift(int shiftArray[], int suffixArray[], string pattern) {
    int patternLength = pattern.size(); 
    int j = suffixArray[0];

    for(int i = 0; i < patternLength; i++) {
        if(shiftArray[i] == 0) {
            shiftArray[i] = j; 
            if(i == j) {
                j = suffixArray[j]; 
            }
        }
    }
}

// Function to find all pattern matches in the text with type flexibility
template <bool UseUnordered>
void findPatternMatches(string text, string pattern, MatchSet<UseUnordered>& matchSet, size_t estimatedMatches) {
    int patternLength = pattern.size(); 
    int textLength = text.size();
    int suffixArray[patternLength + 1];
    int shiftArray[patternLength + 1];

    for(int i = 0; i <= patternLength; i++) {
        shiftArray[i] = 0;
    }

    computePrefixShift(shiftArray, suffixArray, pattern); 
    computeSuffixShift(shiftArray, suffixArray, pattern); 

    // Reserve memory to prevent fragmentation (for unordered_set)
    if constexpr (UseUnordered) {
        matchSet.reserve(estimatedMatches); // Reserve estimated matches
    }

    int shift = 0;
    while(shift <= (textLength - patternLength)) {
        int j = patternLength - 1;
        while(j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        if(j < 0) {
            matchSet.insert(shift); // Insert matching position into the chosen set
            shift += shiftArray[0]; 
        } else {
            shift += shiftArray[j + 1]; 
        }
    }
}

int main() {
    string text, pattern;
    bool useUnordered; // Flag to choose between unordered_set and set
    size_t estimatedMatches; // Variable to hold estimated matches

    cout << "Would you like to use unordered_set instead of set? (1 for yes, 0 for no): ";
    cin >> useUnordered;

    cout << "Enter the original string: ";
    cin >> text;
    cout << "Enter the pattern to search: ";
    cin >> pattern;

    // Estimate the maximal number of matches, for simplicity, we'll assume at most text.size() / pattern.size()
    estimatedMatches = text.size() / pattern.size() + 1;

    MatchSet<true> matchSet; // Use `true` for unordered_set by default; change as per user choice

    // Find all pattern matches in the text
    findPatternMatches<true>(text, pattern, matchSet, estimatedMatches); 

    // Display the positions of pattern matches
    for (const int& position : matchSet) {
        cout << "Pattern found at position: " << position << endl;
    }

    return 0;
}