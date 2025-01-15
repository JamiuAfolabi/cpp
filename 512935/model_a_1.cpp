#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate Levenshtein distance
int levenshteinDistance(const string &s1, const string &s2, int lenS1, int lenS2) {
    // Base cases
    if (lenS1 == 0) return lenS2; // If first string is empty
    if (lenS2 == 0) return lenS1; // If second string is empty

    // If last characters are the same, ignore them and recur for the remaining substring
    if (s1[lenS1 - 1] == s2[lenS2 - 1]) 
        return levenshteinDistance(s1, s2, lenS1 - 1, lenS2 - 1);
    
    // If the last character is different, consider all possibilities and find the minimum
    int insertion = levenshteinDistance(s1, s2, lenS1, lenS2 - 1);
    int deletion = levenshteinDistance(s1, s2, lenS1 - 1, lenS2);
    int substitution = levenshteinDistance(s1, s2, lenS1 - 1, lenS2 - 1);

    return 1 + min(min(insertion, deletion), substitution);
}

// Function to check if a word is in the dictionary
bool isInDictionary(const string &word, const vector<string> &dictionary) {
    for (const string &dictWord : dictionary) {
        if (word == dictWord) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

// Function to suggest similar words if the exact word is not found
void suggestSimilarWords(const string &word, const vector<string> &dictionary) {
    int minDistance = INT_MAX;        // Variable to hold minimum distance found
    string closestMatch;

    for (const string &dictWord : dictionary) {
        int distance = levenshteinDistance(word, dictWord, word.length(), dictWord.length());
        if (distance < minDistance && distance > 0) { // Don't consider exact matches
            minDistance = distance;
            closestMatch = dictWord;
        }
    }

    if (!closestMatch.empty()) {
        cout << "Did you mean: " << closestMatch << "?" << endl;
    } else {
        cout << "No suggestions!" << endl;
    }
}

int main() {
    vector<string> dictionary = {"apple", "banana", "orange", "grape", "peach"};

    string word;
    cout << "Enter a word: ";
    cin >> word;

    if (isInDictionary(word, dictionary)) {
        cout << word << " is in the dictionary." << endl;
    } else {
        cout << word << " is not in the dictionary." << endl;
        suggestSimilarWords(word, dictionary);
    }

    return 0;
}