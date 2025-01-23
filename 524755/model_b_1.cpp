#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int customStringMatch(const string& text, const string& search) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    transform(search.begin(), search.end(), search.begin(), ::tolower);

    size_t len1 = text.length();
    size_t len2 = search.length();

    if (len2 == 0) {
        return 0; // Empty search string, return 0
    }

    for (size_t i = 0; i < len1; i++) {
        if (text[i] == search[0]) {
            size_t j = 1;
            while (i + j < len1 && j < len2) {
                if (text[i + j] == search[j]) {
                    j++;
                } else {
                    break;
                }
            }
            if (j == len2) {
                return i; // Found the search string
            }
        }
    }

    return -1; // Search string not found
}

bool searchProduct(const string& productDescription, const string& searchTerm) {
    int index = customStringMatch(productDescription, searchTerm);
    return index != -1;
}

int main() {
    string productDesc1 = "Smart TV 55 inch 4K Ultra HD HDR Android";
    string productDesc2 = "Wireless Bluetooth Speaker with Bass";
    string productDesc3 = "Apple iPhone 12 128GB Black";

    string searchTerm1 = "ultra hd";
    string searchTerm2 = "bluetooth speaker";
    string searchTerm3 = "iphone 13";
    string searchTerm4 = "";

    cout << "Product Description 1: " << productDesc1 << endl;
    cout << "Search Term 1: " << searchTerm1 << " -> " << (searchProduct(productDesc1, searchTerm1) ? "Found" : "Not Found") << endl;

    cout << "Product Description 2: " << productDesc2 << endl;
    cout << "Search Term 2: " << searchTerm2 << " -> " << (searchProduct(productDesc2, searchTerm2) ? "Found" : "Not Found") << endl;

    cout << "Product Description 3: " << productDesc3 << endl;
    cout << "Search Term 3: " << searchTerm3 << " -> " << (searchProduct(productDesc3, searchTerm3) ? "Found" : "Not Found") << endl;

    cout << "Search Term 4 (empty): " << searchTerm4 << " -> " << (searchProduct(productDesc1, searchTerm4) ? "Found" : "Not Found") << endl;

    return 0;
}