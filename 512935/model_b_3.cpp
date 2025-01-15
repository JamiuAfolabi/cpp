#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate Levenshtein distance using a 2D matrix
int levenshteinDistance(const string &s1, const string &s2) {
    int lenS1 = s1.length();
    int lenS2 = s2.length();

    // Create two vectors to store the intermediate results
    vector<int> dp1(lenS2 + 1);
    vector<int> dp2(lenS2 + 1);

    // Initialize the base cases
    for (int i = 0; i <= lenS2; ++i) {
        dp1[i] = i;
    }

    // Iterate through each character of the first string
    for (int i = 1; i <= lenS1; ++i) {
        dp2[0] = i; // Deletion cost

        // Iterate through each character of the second string
        for (int j = 1; j <= lenS2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp2[j] = min({dp1[j] + 1,    // deletion
                           dp2[j - 1] + 1, // insertion
                           dp1[j - 1] + cost}); // substitution
        }

        // Swap the vectors to prepare for the next iteration
        vector<int> temp = dp1;
        dp1 = dp2;
        dp2 = temp;
    }

    return dp1[lenS2]; // Return the Levenshtein distance
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