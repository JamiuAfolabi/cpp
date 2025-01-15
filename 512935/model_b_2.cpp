#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate Levenshtein distance
int levenshteinDistance(const string &s1, const string &s2) {
    int lenS1 = s1.length();
    int lenS2 = s2.length();

    // Create a 2D array for storing intermediate results
    vector<vector<int>> dp(lenS1 + 1, vector<int>(lenS2 + 1));

    // Initialize the first row and column
    for (int i = 0; i <= lenS1; i++) {
        dp[i][0] = i;
    }

    for (int j = 0; j <= lenS2; j++) {
        dp[0][j] = j;
    }

    // Calculate Levenshtein distance using dynamic programming
    for (int i = 1; i <= lenS1; i++) {
        for (int j = 1; j <= lenS2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // No operation needed if characters are the same
            } else {
                dp[i][j] = 1 + min(dp[i - 1][j] /* Insertion */,  // Insert
                                 dp[i][j - 1] /* Deletion */,   // Delete
                                 dp[i - 1][j - 1]); /* Substitution */ // Replace
            }
        }
    }

    return dp[lenS1][lenS2];
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