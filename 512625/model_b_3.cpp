#include <iostream>
#include <unordered_set>
#include <set>
#include <string>

using namespace std;


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

template <typename Collection>
void findPatternMatches(string text, string pattern, Collection& matchSet) {
    // ... (Algorithm implementation remains the same)
    int patternLength = pattern.size(); 
    int textLength = text.size();
    int suffixArray[patternLength + 1];
    int shiftArray[patternLength + 1];
    
    for(int i = 0; i <= patternLength; i++) {
        shiftArray[i] = 0;
    }
    
    computePrefixShift(shiftArray, suffixArray, pattern); 
    computeSuffixShift(shiftArray, suffixArray, pattern); 
    
    int shift = 0;
    while(shift <= (textLength - patternLength)) {
        int j = patternLength - 1;
        while(j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        if(j < 0) {
            matchSet.insert(shift); // Insert matching position into unordered_set
            shift += shiftArray[0]; 
        } else {
            shift += shiftArray[j + 1]; 
        }
    }
}

int main() {
    string text, pattern;

    cout << "Enter the original string: ";
    cin >> text;
    cout << "Enter the pattern to search: ";
    cin >> pattern;

    // User choice between unordered_set and set
    char choice;
    cout << "Choose data structure to store matches ('u' for unordered_set, 's' for set): ";
    cin >> choice;

    using MatchCollection = typename std::conditional_t<choice == 'u', unordered_set<int>, set<int>>;
    MatchCollection matchSet;

    // Find all pattern matches in the text using the chosen data structure
    findPatternMatches(text, pattern, matchSet);

    // Display the positions of pattern matches
    for (const int& position : matchSet) {
        cout << "Pattern found at position: " << position << endl;
    }

    return 0;
}
