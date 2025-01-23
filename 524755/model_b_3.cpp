#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>

const int MAX_DISTANCE = 2;  // Maximum Levenshtein distance allowed

bool is_match_within_distance(const std::string& text, const std::string& pattern, int max_distance) {
    int m = text.size();
    int n = pattern.size();

    int dp[m + 1][n + 1];

    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }

    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text[i - 1] == pattern[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min(dp[i - 1][j] + 1, // Insert
                                    dp[i][j - 1] + 1, // Delete
                                    dp[i - 1][j - 1] + 1); // Replace
            }
        }
    }

    return dp[m][n] <= max_distance;
}

class ProductSearch {
private:
   static std::string normalize(std::string str) {
       std::string result;
       for (char c : str) {
           if (std::isalnum(c)) {
               result += std::tolower(c);
           } else {
               result += ' ';
           }
       }
       return result;
   }

   static bool customMatch(const std::string& text, const std::string& pattern) {
       ... // Same as before
   }

public:
    struct SearchResult {
        std::string product;
        int relevance_score;
    };

    static std::vector<SearchResult> search(const std::vector<std::string>& products, const std::string& searchTerm) {
        std::vector<SearchResult> results;
        std::string normalizedSearch = normalize(searchTerm);
        std::istringstream iss(normalizedSearch);
        std::vector<std::string> searchWords;
        std::string word;
        while (iss >> word) {
            if (!word.empty()) {
                searchWords.push_back(word);
            }
        }
  
        for (const auto& product : products) {
            std::string normalizedProduct = normalize(product);
            int score = 0;

            // Combine Boyer-Moore with Levenshtein distance check for fuzziness
            for (const auto& searchWord : searchWords) {
                if (customMatch(normalizedProduct, searchWord)) {
                    score++;
                } 
                else {
                    for (auto it = std::begin(normalizedProduct.words); it != std::end(normalizedProduct.words); ++it) {
                        if (is_match_within_distance(*it, searchWord, MAX_DISTANCE)) {
                            score++;
                            break;
                        }
                    }
                }
            }

            if (score > 0) {
                results.push_back({product, score});
            }
        }

        std::sort(results.begin(), results.end());
        return results;
    }
};

int main() {
    std::vector<std::string> products = {
        "Apple iPhone 14 Pro (256GB)",
        "Samsung Galaxy S21 Ultra 5G!",
        "Google Pixel-6 Black Edition",
        "OnePlus 9 Pro @ $899.99",
        "Sony WH-1000XM4 [Wireless]",
        "LG OLED TV 65\"",
        "Apple iPad Air & Pencil"
    };

    std::string searchTerm;
    std::cout << "Enter search term: ";
    std::getline(std::cin, searchTerm);

    auto results = ProductSearch::search(products, searchTerm);
    
    std::cout << "\nResults:\n";
    for (const auto& result : results) {
        std::cout << "- " << result.product << " (Relevance Score: " << result.relevance_score << ")\n";
    }

    return 0;
}