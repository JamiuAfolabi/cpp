#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate Levenshtein distance using a 2D matrix
int levenshteinDistance(const string &s1, const string &s2) {
    int lenS1 = s1.length();
    int lenS2 = s2.length();
    
    // Create a 2D matrix to store distance values
    vector<vector<int>> dp(lenS1 + 1, vector<int>(lenS2 + 1));

    // Initialize the base cases
    for (int i = 0; i <= lenS1; ++i) {
        dp[i][0] = i; // Deletion cost
    }
    for (int j = 0; j <= lenS2; ++j) {
        dp[0][j] = j; // Insertion cost
    }

    // Fill the matrix
    for (int i = 1; i <= lenS1; ++i) {
        for (int j = 1; j <= lenS2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1; // Cost of substitution
            dp[i][j] = min({dp[i - 1][j] + 1,        // Deletion
                             dp[i][j - 1] + 1,        // Insertion
                             dp[i - 1][j - 1] + cost}); // Substitution
        }
    }

    return dp[lenS1][lenS2]; // Return the Levenshtein distance
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
        int distance = levenshteinDistance(word, dictWord);
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