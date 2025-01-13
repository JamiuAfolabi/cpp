#include<bits/stdc++.h>
using namespace std;

// Function to find the last occurrence of a character in the pattern
int lastOccurrence(string pattern, char ch) {
    for (int i = pattern.size() - 1; i >= 0; i--)
        if (pattern[i] == ch)
            return i;
    return -1;
}

// Function to calculate bad character shift values
vector<int> badCharShift(string pattern)
{
    int n = pattern.size();
    vector<int> bad_char(128, n);

    for (int i = 0; i < n; i++) {
        bad_char[pattern[i]] = n - i - 1;
    }

    return bad_char;
}

// Function to calculate good suffix shift values
vector<int> goodSuffixShift(string pattern)
{
    int n = pattern.size();
    vector<int> good_suffix(n, 0);

    for (int i = n - 2; i >= 0; i--) {
        int j = lastOccurrence(pattern.substr(i + 1), pattern[i]);
        if (j != -1)
            good_suffix[i] = j - i;
        else
            good_suffix[i] = n - i - 1;
    }

    return good_suffix;
}

// Function to perform Boyer-Moore string search
void boyerMooreSearch(string text, string pattern)
{
    if (pattern.empty()) {
        cout << "Pattern found at index 0." << endl;
        return;
    }

    if (text.empty()) {
        cout << "Pattern not found." << endl;
        return;
    }

    int m = text.size();
    int n = pattern.size();

    vector<int> bad_char = badCharShift(pattern);
    vector<int> good_suffix = goodSuffixShift(pattern);

    int i = 0;
    while (i <= m - n)
    {
        int j = n - 1;

        // Compare the pattern from the rightmost character
        while (j >= 0 && text[i + j] == pattern[j])
            j--;

        if (j < 0) {
            cout << "Pattern found at index " << i << endl;
            i += 1;
        } 
        else {
			// Calculate shift using bad character rule and good suffix rule
            int shift = max(1, bad_char[text[i + j]]);
			
            // Check if good suffix rule can give a better shift
            int good_suffix_shift = good_suffix[j];
            if (i + j < m && good_suffix_shift < shift &&
                text.substr(i + j - good_suffix_shift + 1, good_suffix_shift) == pattern.substr(0, good_suffix_shift))
                shift = good_suffix_shift;

            i += shift;
        }
    }

    cout << "Pattern not found." << endl;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string text, pattern;
    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the pattern: ";
    getline(cin, pattern);

    boyerMooreSearch(text, pattern);
    return 0;
}