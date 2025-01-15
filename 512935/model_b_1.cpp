#include <string>
#include <set>
#include <iterator>
#include <iostream>
using namespace std;

// Levenshtein distance function
int levenshteinDistance(const string& word1, const string& word2) {
    int m = word1.size();
    int n = word2.size();

    // special cases: two empty strings and one empty string
    if (m == 0)
        return n;
    if (n == 0)
        return m;
    // dynamic programming table to store the distances between respective characters in both words
    int dp[m + 1][n + 1];
    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // if char are same, no change in distance from previous characters
            } else {
                dp[i][j] = min(dp[i - 1][j] + 1, // insertion
                               dp[i][j - 1] + 1, // deletion
                               dp[i - 1][j - 1] + 1); // substitution
            }
        }
    return dp[m][n];
}

 bool findApproximateWord(const set<string>& dictionary, const string& query, int maxDistance)
{
   
    for (const string& word : dictionary) {

        if (levenshteinDistance(word, query) <= maxDistance) {
	    cout << word << " -( distance " << levenshteinDistance(word, query) << ")" <<endl;
	    return true;
        }
    }
    return false;
}

int main()
{
    set<string> dictionary = {
        "popular", "acceptable", "conditional", "custom", "historical", "similar"
    };

    string query = "custom";
    int maxDistance = 1;   // You can adjust this value as per your error tolerance

    if (findApproximateWord(dictionary, query, maxDistance)) {
	cout<< "Query Found" <<endl;
    } else {
        cout << "Query not found" << endl;
    }
    return 0;
}