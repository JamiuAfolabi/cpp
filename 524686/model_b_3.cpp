
#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

// Function to display the daily specials
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

// Function to remove emojis from a string
string removeEmojis(const string& input) {
    string emoji_free_text;
    
    // Regular expression to match emojis
    regex emoji_regex("\\p{Emoji}");
    sregex_iterator rit(input.begin(), input.end(), emoji_regex);
    sregex_iterator end;
    
    while (rit != end) {
        emoji_free_text.append(input, 0, rit->prematch().length());
        rit++;
    }
    emoji_free_text.append(input, rit->str().length(), input.length());
    
    return emoji_free_text;
}

// Function to add a new special
void addSpecial(vector<string>& specials, const string& newName) {
    specials.push_back(removeEmojis(newName));  // Remove emojis before adding
}

// Function to record and display customer feedback
void recordFeedback(vector<string>& feedbacks) {
    string review;
    cout << "Enter customer feedback (or 'done' to exit): ";
    getline(cin, review);
    
    while (review != "done") {
        feedbacks.push_back(removeEmojis(review));
        cout << "Enter customer feedback (or 'done' to exit): ";
        getline(cin, review);
    }
    
    cout << "\nCustomer Feedback (Emojis Removed):" << endl;
    for (size_t i = 0; i < feedbacks.size(); ++i) {
        cout << i + 1 << ". " << feedbacks[i] << endl;
    }
}

int main() {
    // Vector to store daily specials
    vector<string> dailySpecials;

    // Add initial specials
    addSpecial(dailySpecials, "Grilled Salmon");
    addSpecial(dailySpecials, "Chicken Alfredo");
    addSpecial(dailySpecials, "Veggie Burger");

    cout << endl << "Updated Specials:" << endl;
    displayDailySpecials(dailySpecials);

    // Add some sample feedback with emojis
    vector<string> feedbacks;
    recordFeedback(feedbacks);

    return 0;
}
 