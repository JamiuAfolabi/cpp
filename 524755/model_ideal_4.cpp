#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <future>

class ProductSearch {
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
                                          const std::string& searchTerm,
                                          size_t numThreads = std::thread::hardware_concurrency());

private:
    static const int MAX_DISTANCE = 2;

    static std::unordered_map<char, int> buildBadCharTable(const std::string& pattern);
    static bool boyerMooreSearch(const std::string& text, const std::string& pattern);
    static int levenshteinDistance(const std::string& s1, const std::string& s2);
    static std::string normalize(const std::string& str);
    static std::vector<std::string> tokenize(const std::string& str);
    
    static void searchChunk(const std::vector<std::string>& products,
                          size_t start, size_t end,
                          const std::string& normalizedSearch,
                          const std::vector<std::string>& searchWords,
                          std::vector<SearchResult>& results,
                          std::mutex& resultsMutex);
};

// Method implementations
std::unordered_map<char, int> ProductSearch::buildBadCharTable(const std::string& pattern) {
    std::unordered_map<char, int> badChar;
    int length = pattern.length();
    for (int i = 0; i < length - 1; i++) {
        badChar[std::tolower(pattern[i])] = length - 1 - i;
    }
    return badChar;
}

bool ProductSearch::boyerMooreSearch(const std::string& text, const std::string& pattern) {
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

int ProductSearch::levenshteinDistance(const std::string& s1, const std::string& s2) {
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

std::string ProductSearch::normalize(const std::string& str) {
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

std::vector<std::string> ProductSearch::tokenize(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

void ProductSearch::searchChunk(const std::vector<std::string>& products,
                              size_t start, size_t end,
                              const std::string& normalizedSearch,
                              const std::vector<std::string>& searchWords,
                              std::vector<SearchResult>& results,
                              std::mutex& resultsMutex) {
    std::vector<SearchResult> localResults;
    
    for (size_t i = start; i < end; i++) {
        const auto& product = products[i];
        std::string normalizedProduct = normalize(product);
        auto productWords = tokenize(normalizedProduct);
        
        float bestSimilarity = 0.0f;
        int matches = 0;
        bool hasExactMatch = false;
        
        for (const auto& searchWord : searchWords) {
            bool wordMatched = false;
            
            for (const auto& productWord : productWords) {
                if (boyerMooreSearch(productWord, searchWord)) {
                    matches++;
                    wordMatched = true;
                    hasExactMatch = true;
                    bestSimilarity = 1.0f;
                    break;
                }
            }
            
            if (!wordMatched) {
                for (const auto& productWord : productWords) {
                    int distance = levenshteinDistance(searchWord, productWord);
                    float similarity = 1.0f - (float)distance / 
                                     std::max(productWord.length(), searchWord.length());
                    
                    if (distance <= MAX_DISTANCE) {
                        matches++;
                        bestSimilarity = std::max(bestSimilarity, similarity);
                        break;
                    }
                }
            }
        }
        
        if (matches > 0) {
            localResults.push_back({product, matches, bestSimilarity, hasExactMatch});
        }
    }
    
    std::lock_guard<std::mutex> lock(resultsMutex);
    results.insert(results.end(), localResults.begin(), localResults.end());
}

std::vector<ProductSearch::SearchResult> ProductSearch::search(
    const std::vector<std::string>& products,
    const std::string& searchTerm,
    size_t numThreads) {
    
    if (numThreads == 0) numThreads = 1;
    
    std::vector<SearchResult> results;
    std::mutex resultsMutex;
    std::vector<std::thread> threads;
    
    std::string normalizedSearch = normalize(searchTerm);
    auto searchWords = tokenize(normalizedSearch);
    
    std::cout << "Normalized search term: " << normalizedSearch << "\n";
    std::cout << "Search words: ";
    for (const auto& word : searchWords) std::cout << word << " ";
    std::cout << "\n\n";
    
    size_t chunkSize = products.size() / numThreads;
    size_t remainder = products.size() % numThreads;
    size_t start = 0;
    
    for (size_t i = 0; i < numThreads; i++) {
        size_t end = start + chunkSize + (i < remainder ? 1 : 0);
        
        threads.emplace_back(searchChunk,
                           std::ref(products),
                           start,
                           end,
                           std::ref(normalizedSearch),
                           std::ref(searchWords),
                           std::ref(results),
                           std::ref(resultsMutex));
        
        start = end;
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::sort(results.begin(), results.end());
    return results;
}

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