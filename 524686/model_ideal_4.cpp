#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::transform and std::search
using namespace std;

// Function to convert string to lowercase for case-insensitive search
string toLowerCase(string str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Function to search dishes by partial name
vector<string> searchDishes(const vector<string>& dishes, const string& searchTerm) {
    vector<string> matches;
    string lowerSearchTerm = toLowerCase(searchTerm);
    
    for (const string& dish : dishes) {
        string lowerDish = toLowerCase(dish);
        // Check if the lowercase dish name contains the lowercase search term
        if (lowerDish.find(lowerSearchTerm) != string::npos) {
            matches.push_back(dish);  // Add original dish name to matches
        }
    }
    return matches;
}

// Function to display daily specials
void displayDailySpecials(const vector<string>& specials) {
    cout << "Today's Specials:" << endl;
    if (specials.empty()) {
        cout << "No specials are available." << endl;
        return;
    }
    for (size_t i = 0; i < specials.size(); ++i) {
        cout << i + 1 << ". " << specials[i] << endl;
    }
}

// Function to modify a daily special
void modifySpecial(vector<string>& specials, size_t index, const string& newName) {
    if (index < specials.size()) {
        specials[index] = newName;  // Update the special
    } else {
        cout << "Invalid index for modification!" << endl;
    }
}

// Function to add a new special
void addSpecial(vector<string>& specials, const string& newName) {
    specials.push_back(newName);  // Add a new special to the list
}

// Function to remove emojis from feedback
string removeEmojis(const string& feedback) {
    string cleanedFeedback;
    // Copy only ASCII characters (0-127)
    for (char c : feedback) {
        if (c >= 0 && c <= 127) { // ASCII range
            cleanedFeedback += c;
        }
    }
    return cleanedFeedback;
}

// Function to display customer feedback
void displayFeedback(const vector<string>& feedbacks) {
    cout << "Customer Feedback:" << endl;
    if (feedbacks.empty()) {
        cout << "No feedback available." << endl;
        return;
    }
    for (size_t i = 0; i < feedbacks.size(); ++i) {
        cout << i + 1 << ". " << feedbacks[i] << endl;
    }
}

int main() {
    // Vector to store daily specials
    vector<string> dailySpecials;
    
    // Add initial specials with more varied dishes including pizzas
    addSpecial(dailySpecials, "Margherita Pizza");
    addSpecial(dailySpecials, "Pepperoni Pizza");
    addSpecial(dailySpecials, "Grilled Salmon");
    addSpecial(dailySpecials, "BBQ Chicken Pizza");
    addSpecial(dailySpecials, "Veggie Burger");
    addSpecial(dailySpecials, "Hawaiian Pizza");
    
    // Display all specials
    cout << "All Available Dishes:" << endl;
    displayDailySpecials(dailySpecials);
    
    // Test the search functionality
    string searchTerm = "pizza";
    cout << "\nSearching for dishes containing '" << searchTerm << "':" << endl;
    vector<string> searchResults = searchDishes(dailySpecials, searchTerm);
    
    if (searchResults.empty()) {
        cout << "No matches found." << endl;
    } else {
        cout << "Found " << searchResults.size() << " matches:" << endl;
        for (const string& dish : searchResults) {
            cout << "- " << dish << endl;
        }
    }
    
    // Test case-insensitive search
    searchTerm = "PIZZA";
    cout << "\nTesting case-insensitive search for '" << searchTerm << "':" << endl;
    searchResults = searchDishes(dailySpecials, searchTerm);
    
    if (!searchResults.empty()) {
        cout << "Found " << searchResults.size() << " matches:" << endl;
        for (const string& dish : searchResults) {
            cout << "- " << dish << endl;
        }
    }
    
    return 0;
}