#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath> // Added for log calculation

class ProductSearch {
private:
   // Convert string to lowercase and handle special chars
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

   // Custom string match with Boyer-Moore algorithm 
   static bool customMatch(const std::string& text, const std::string& pattern) {
       if (pattern.empty()) return true;
       
       std::vector<int> last(256, -1);
       for (int i = 0; i < pattern.length(); i++) {
           last[pattern[i]] = i;
       }
       
       int i = pattern.length() - 1;
       int j = pattern.length() - 1;
       
       while (i < text.length()) {
           if (text[i] == pattern[j]) {
               if (j == 0) return true;
               i--; j--;
           } else {
               i += pattern.length() - std::min(j, 1 + last[text[i]]);
               j = pattern.length() - 1;
           }
       }
       return false;
   }

public:
  static struct SearchResult {
      std::string product;
      int relevanceScore;
  };

  static std::vector<SearchResult> searchWithRelevance(const std::vector<std::string>& products, const std::string& searchTerm) {
      // other code remains the same
      //Functionality to calculate relevance and update results
      for (int i = 0; i < results.size(); i++){
          int relevanceScore = std::count(searchWords.begin(), searchWords.end(), \
                              [results[i].product](const std::string& searchWord) {
                                  return customMatch(normalize(results[i].product), searchWord);
                              });
          results[i].relevanceScore = relevanceScore;
          if(relevanceScore == 0) {
              results.erase(results.begin() + i);
              i--;
          }
      }

      std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
          return a.relevanceScore > b.relevanceScore;
      });
      return results;
  }
};

int main() {
  std::vector<std::string> products = {
      "XYZ_  Smartphone 5G Ultra",
      "Cheap Laptop bundled with monitor",
      "Wireless airpods Pro elite",
      "Laptop bag Laptop underwear shirt",
      "LG BlackSaturn TV 55-inch 4K"
  };
  std::string searchTerm;
  std::cout << "Enter search term: ";
  std::getline(std::cin, searchTerm);

  auto results = ProductSearch::searchWithRelevance(products, searchTerm);

  std::cout << "\nResults with Relevance Scores:\n";
  if (results.empty()) {
      std::cout << "No matches found." << std::endl;
  } else {
      for (const auto& result : results) {
          std::cout << "- " << result.product << " - Relevance Score: " << result.relevanceScore << std::endl;
      }
  }
  return 0;
}