#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_map>
#include <functional>
#include <future>
#include <atomic>

// Rest of the ProductSearch class remains the same
class ProductSearch {
private:
    static const int CHUNK_SIZE = 100; // Adjust the chunk size as needed
    static const int MAX_DISTANCE = 2;

    struct ChunkSearchResult {
        std::vector<SearchResult> results;
        int chunkSize;
    };
    static std::unordered_map<char, int> buildBadCharTable(const std::string& pattern) {
        std::unordered_map<char, int> badChar;
        int length = pattern.length();
        for (int i = 0; i < length - 1; i++) {
            badChar[std::tolower(pattern[i])] = length - 1 - i;
        }
        return badChar;
    }

    static bool boyerMooreSearch(const std::string& text, const std::string& pattern) {
        std::string lowerText = text;
        std::string lowerPattern = pattern;
        std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
        std::transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);
        
        auto badChar = buildBadCharTable(lowerPattern);
        int shift = 0;
        
        if (lowerText.length() < lowerPattern.length()) return false;
    
    while (shift <= (int)(lowerText.length() - lowerPattern.length())) {
            try {
                if (lowerText.substr(shift, lowerPattern.length()) == lowerPattern) {
                    return true;
                }
                
                char nextChar = shift + lowerPattern.length() < lowerText.length() ? 
                              std::tolower(lowerText[shift + lowerPattern.length()]) : '\0';
                    shift += badChar.count(nextChar) ? badChar[nextChar] : lowerPattern.length();
            } catch (const std::out_of_range& e) {
                return false;
            }
        }
        return false;
    }

    static int levenshteinDistance(const std::string& s1, const std::string& s2) {
        std::vector<std::vector<int>> dp(s1.length() + 1, std::vector<int>(s2.length() + 1));
        
        for (int i = 0; i <= s1.length(); i++) dp[i][0] = i;
        for (int j = 0; j <= s2.length(); j++) dp[0][j] = j;
        
        for (int i = 1; i <= s1.length(); i++) {
            for (int j = 1; j <= s2.length(); j++) {
                if (std::tolower(s1[i-1]) == std::tolower(s2[j-1])) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                }
            }
        }
        return dp[s1.length()][s2.length()];
    }

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

    static std::vector<std::string> tokenize(const std::string& str) {
        std::istringstream iss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }

public:
    struct SearchResult {
        std::string product;
        int relevance_score;
        float similarity;
        bool exact_match;
        
        bool operator<(const SearchResult& other) const {
            if (exact_match != other.exact_match) return exact_match > other.exact_match;
            if (abs(similarity - other.similarity) > 0.1f) return similarity > other.similarity;
            return relevance_score > other.relevance_score;
        }
    };
    static std::vector<SearchResult> search(const std::vector<std::string>& products,
                                           const std::string& searchTerm) {

    // Calculate the number of chunks and initialize the results array
        int numChunks = (products.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;
        std::vector<std::future<ChunkSearchResult>> futures(numChunks);
        std::atomic<int> chunkStart(0);

        // Launch asynchronous searches for each chunk
        for (int chunkIndex = 0; chunkIndex < numChunks; ++chunkIndex) {
            int chunkEnd = std::min(chunkStart + CHUNK_SIZE, products.size());
            futures[chunkIndex] = std::async(std::launch::async, searchChunk, products,
                                              searchTerm, chunkStart, chunkEnd);
            chunkStart += CHUNK_SIZE;
        }
        
        // Collect and merge results from all chunks
        std::vector<SearchResult> allResults;
        for (int chunkIndex = 0; chunkIndex < numChunks; ++chunkIndex) {
            ChunkSearchResult chunkResult = futures[chunkIndex].get();
            allResults.insert(allResults.end(), chunkResult.results.begin(),
                              chunkResult.results.end());
        }

        // Sort results based on the score 
        std::sort(allResults.begin(), allResults.end());
        return allResults;
    }

private:
    static ChunkSearchResult searchChunk(const std::vector<std::string>& products,
                                        const std::string& searchTerm,
                                        int chunkStart, int chunkEnd) {
        ChunkSearchResult result;
        result.chunkSize = chunkEnd - chunkStart;

        std::string normalizedSearch = normalize(searchTerm);
        auto searchWords = tokenize(normalizedSearch); 

        for (int i = chunkStart; i < chunkEnd; ++i) {
            std::string normalizedProduct = normalize(products[i]);
            auto productWords = tokenize(normalizedProduct);
           
            float bestSimilarity = 0.0f;
            int matches = 0;
            bool hasExactMatch = false;
            
            // (rest of the search logic remains the same)

            if (matches > 0) {
                result.results.push_back({products[i], matches, bestSimilarity, hasExactMatch});
            }
        }
        return result;
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
    
    std::cout << "Results:\n";
    for (const auto& result : results) {
        std::cout << "- " << result.product 
                 << " (Score: " << result.relevance_score 
                 << ", Similarity: " << result.similarity 
                 << ", Exact: " << (result.exact_match ? "Yes" : "No") << ")\n";
    }

    return 0;
}
