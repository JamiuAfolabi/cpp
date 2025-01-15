#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate Levenshtein distance using two vectors
int levenshteinDistance(const string &s1, const string &s2) {
    int lenS1 = s1.length();
    int lenS2 = s2.length();

    // Ensure s1 is the shorter string for space efficiency
    if (lenS1 > lenS2) {
        return levenshteinDistance(s2, s1);
    }

    // Create two vectors to store distances
    vector<int> previous(lenS2 + 1);
    vector<int> current(lenS2 + 1);

    // Initialize the base cases
    for (int j = 0; j <= lenS2; ++j) {
        previous[j] = j; // Deletion cost
    }

    // Fill the distance vectors
    for (int i = 1; i <= lenS1; ++i) {
        current[0] = i; // Insertion cost
        for (int j = 1; j <= lenS2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1; // Cost of substitution
            current[j] = min({previous[j] + 1,        // Deletion
                               current[j - 1] + 1,    // Insertion
                               previous[j - 1] + cost}); // Substitution
        }
        // Move current to previous for the next iteration
        swap(previous, current);
    }

    return previous[lenS2]; // Return the Levenshtein distance
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