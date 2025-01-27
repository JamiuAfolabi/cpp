#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>  // For tolower

using namespace std;

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

void modifySpecial(vector<string>& specials, size_t index, const string& newName) {
    if (index < specials.size()) {
        specials[index] = newName;
    } else {
        cout << "Invalid index for modification!" << endl;
    }
}

void addSpecial(vector<string>& specials, const string& newName) {
    specials.push_back(newName);
}

string removeEmojis(const string& feedback) {
    string cleanedFeedback;
    for (char c : feedback) {
        if (c >= 0 && c <= 127) {
            cleanedFeedback += c;
        }
    }
    return cleanedFeedback;
}

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

// Case-insensitive search function
void searchSpecial(const vector<string>& specials, const string& searchTerm) {
    cout << endl << "Search Results for '" << searchTerm << "':" << endl;
    
    // Convert search term to lowercase
    string lowerTerm = searchTerm;
    transform(lowerTerm.begin(), lowerTerm.end(), lowerTerm.begin(), ::tolower);

    bool found = false;
    for (size_t i = 0; i < specials.size(); ++i) {
        // Convert special name to lowercase
        string lowerSpecial = specials[i];
        transform(lowerSpecial.begin(), lowerSpecial.end(), lowerSpecial.begin(), ::tolower);

        if (lowerSpecial.find(lowerTerm) != string::npos) {
            found = true;
            cout << i + 1 << ". " << specials[i] << endl;
        }
    }

    if (!found) {
        cout << "No matching specials found." << endl;
    }
}

int main() {
    vector<string> dailySpecials;
    addSpecial(dailySpecials, "Grilled Salmon");
    addSpecial(dailySpecials, "Chicken Alfredo");
    addSpecial(dailySpecials, "Veggie Burger");
    addSpecial(dailySpecials, "Margherita Pizza");
    addSpecial(dailySpecials, "Pepperoni Pizza");

    displayDailySpecials(dailySpecials);

    modifySpecial(dailySpecials, 1, "Shrimp Alfredo");
    cout << endl << "Updated Specials:" << endl;
    displayDailySpecials(dailySpecials);

    vector<string> customerFeedbacks;
    customerFeedbacks.push_back("Amazing food! 🍣🍰");
    customerFeedbacks.push_back("Will come again! 😊");
    customerFeedbacks.push_back("Not great... 😞");

    for (string& feedback : customerFeedbacks) {
        feedback = removeEmojis(feedback);
    }
    displayFeedback(customerFeedbacks);

    // Search functionality
    string searchQuery;
    cout << "\nEnter a dish name to search: ";
    getline(cin, searchQuery);
    searchSpecial(dailySpecials, searchQuery);