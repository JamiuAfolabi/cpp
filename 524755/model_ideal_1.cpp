#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

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
   static std::vector<std::string> search(const std::vector<std::string>& products, 
                                        const std::string& searchTerm) {
       std::vector<std::string> results;
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

       // Search products
       for (const auto& product : products) {
           std::string normalizedProduct = normalize(product);
           bool matchFound = true;

           // Check if all search words are in product
           for (const auto& searchWord : searchWords) {
               if (!customMatch(normalizedProduct, searchWord)) {
                   matchFound = false;
                   break;
               }
           }

           if (matchFound) {
               results.push_back(product);
           }
       }
       
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
       std::cout << "- " << result << "\n";
   }

   return 0;
}