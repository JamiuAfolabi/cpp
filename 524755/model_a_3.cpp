#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>

class ProductSearch {
private:
    // Convert string to lowercase and handle special chars
    static std::string normalize(const std::string& str) {
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

    // Levenshtein distance function
    static int levenshtein(const std::string& s1, const std::string& s2) {
        const size_t len1 = s1.size();
        const size_t len2 = s2.size();

        std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

        for (size_t i = 0; i <= len1; ++i) d[i][0] = i;
        for (size_t j = 0; j <= len2; ++j) d[0][j] = j;

        for (size_t i = 1; i <= len1; ++i) {
            for (size_t j = 1; j <= len2; ++j) {
                int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
                d[i][j] = std::min({
                    d[i - 1][j] + 1,         // Deletion
                    d[i][j - 1] + 1,         // Insertion
                    d[i - 1][j - 1] + cost   // Substitution
                });
            }
        }

        return d[len1][len2];
    }

    // Check if a product matches using Boyer-Moore or fuzzy matching
    static bool matchProduct(const std::string& product, const std::string& searchWord, int threshold) {
        // First, check for exact match using Boyer-Moore or contains
        if (product.find(searchWord) != std::string::npos) {
            return true; // Exact match found
        }
        
        // If no exact match, check for fuzzy match using Levenshtein distance
        if (levenshtein(normalize(product), searchWord) <= threshold) {
            return true; // Fuzzy match found
        }

        return false;
    }

public:
    struct SearchResult {
        std::string product;
        int relevance_score;

        // Comparator for sorting search results by relevance score
        bool operator<(const SearchResult& other) const {
            return relevance_score > other.relevance_score; // sorted in descending order
        }
    };

    // Function to perform the search
    static std::vector<SearchResult> search(const std::vector<std::string>& products,
                                             const std::string& searchTerm,
                                             int fuzzyThreshold) {
        std::vector<SearchResult> results;
        std::string normalizedSearch = normalize(searchTerm);
        
        // Split search into words
        std::istringstream iss(normalizedSearch);
        std::vector<std::string> searchWords;
        std::string word;
        while (iss >> word) {
            if (!word.empty()) {
                searchWords.push_back(word);
            }
        }

        // Search products and calculate relevance scores
        for (const auto& product : products) {
            std::string normalizedProduct = normalize(product);
            int score = 0;

            for (const auto& searchWord : searchWords) {
                if (matchProduct(normalizedProduct, searchWord, fuzzyThreshold)) {
                    score++;
                }
            }

            if (score > 0) {
                results.push_back({product, score});
            }
        }

        // Sort results based on relevance score
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

    // Set the Levenshtein distance threshold for fuzzy matching (adjust as needed)
    int fuzzyThreshold = 10;

    auto results = ProductSearch::search(products, searchTerm, fuzzyThreshold);
    
    std::cout << "\nResults:\n";
    for (const auto& result : results) {
        std::cout << "- " << result.product << " (Relevance Score: " << result.relevance_score << ")\n";
    }

    return 0;
}