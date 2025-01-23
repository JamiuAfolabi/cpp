#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// Function to convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

// Function to perform custom string matching
std::vector<std::string> searchProducts(const std::vector<std::string>& products, const std::string& searchTerm) {
    std::vector<std::string> matchedProducts;
    // Convert the search term to lowercase
    std::string lowerSearchTerm = toLower(searchTerm);

    for (const auto& product : products) {
        // Convert product description to lowercase
        std::string lowerProduct = toLower(product);
        // Check for case-insensitive partial match
        if (lowerProduct.find(lowerSearchTerm) != std::string::npos) {
            matchedProducts.push_back(product); // Add the original description
        }
    }
    return matchedProducts;
}

int main() {
    // Sample product descriptions
    std::vector<std::string> products = {
        "Apple iPhone 14 Pro",
        "Samsung Galaxy S21",
        "Google Pixel 6",
        "OnePlus 9 Pro",
        "Sony WH-1000XM4 Headphones",
        "LG OLED TV",
        "Apple iPad Air"
    };

    // Search term
    std::string searchTerm;
    std::cout << "Enter product search term: ";
    std::getline(std::cin, searchTerm);

    // Get matched products
    std::vector<std::string> results = searchProducts(products, searchTerm);
    
    // Display results
    if (results.empty()) {
        std::cout << "No products found matching your search." << std::endl;
    } else {
        std::cout << "Matched Products:" << std::endl;
        for (const auto& result : results) {
            std::cout << "- " << result << std::endl;
        }
    }

    return 0;
}