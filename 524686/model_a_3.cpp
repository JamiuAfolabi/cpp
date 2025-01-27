#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::remove_if

using namespace std;

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

    // Add initial specials
    addSpecial(dailySpecials, "Grilled Salmon");
    addSpecial(dailySpecials, "Chicken Alfredo");
    addSpecial(dailySpecials, "Veggie Burger");

    // Display the daily specials
    displayDailySpecials(dailySpecials);

    // Modify a daily special
    modifySpecial(dailySpecials, 1, "Shrimp Alfredo");

    cout << endl << "Updated Specials:" << endl;
    displayDailySpecials(dailySpecials);

    // Adding a new special
    addSpecial(dailySpecials, "Chocolate Lava Cake");

    cout << endl << "Final Specials:" << endl;
    displayDailySpecials(dailySpecials);

    // Handling customer feedback
    vector<string> customerFeedbacks;

    // Simulating customer feedback with emojis
    customerFeedbacks.push_back("Amazing food! 🍣🍰");
    customerFeedbacks.push_back("Will come again! 😊");
    customerFeedbacks.push_back("Not great... 😞");
    
    // Clean and store feedback
    for (string& feedback : customerFeedbacks) {
        feedback = removeEmojis(feedback); // Remove emojis
    }

    // Displaying cleaned feedback
    cout << endl;
    displayFeedback(customerFeedbacks);

    return 0;
}