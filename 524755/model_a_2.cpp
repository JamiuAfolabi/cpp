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

    // Check if a pattern matches a text
    static bool containsWord(const std::string& text, const std::string& pattern) {
        return text.find(pattern) != std::string::npos;
    }

public:
    struct SearchResult {
        std::string product;
        int relevance_score;

        // Define a comparator for sorting search results by relevance score
        bool operator<(const SearchResult& other) const {
            return relevance_score > other.relevance_score; // sorted in descending order
        }
    };

    // Function to perform the search
    static std::vector<SearchResult> search(const std::vector<std::string>& products,
                                             const std::string& searchTerm) {
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
                if (containsWord(normalizedProduct, searchWord)) {
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

    auto results = ProductSearch::search(products, searchTerm);
    
    std::cout << "\nResults:\n";
    for (const auto& result : results) {
        std::cout << "- " << result.product << " (Relevance Score: " << result.relevance_score << ")\n";
    }

    return 0;
}