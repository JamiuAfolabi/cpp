#include <iostream>
#include <string>
#include <vector>

bool isMatch(const std::string& text, const std::string& pattern) {
    int n = text.size();
    int m = pattern.size();
    
    // Create a 2D DP table
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));
    
    // Empty pattern matches empty text
    dp[0][0] = true;
    
    // Handle patterns starting with '*'
    for (int j = 1; j <= m; j++) {
        if (pattern[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1]; // '*' can match zero characters
        }
    }
    
    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (pattern[j - 1] == text[i - 1] || pattern[j - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1]; // Match single character or '?'
            } else if (pattern[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j]; // '*' matches zero or one character
            }
        }
    }
    
    return dp[n][m];
}

int main() {
    std::string text = "ababacabcabababd";
    std::string pattern = "a*b?c";
    
    if (isMatch(text, pattern)) {
        std::cout << "The pattern matches the text." << std::endl;
    } else {
        std::cout << "The pattern does not match the text." << std::endl;
    }

    return 0;
}