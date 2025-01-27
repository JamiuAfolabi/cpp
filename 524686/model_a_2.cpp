#include <iostream>
#include <vector>
#include <string>  // For C++ string functionalities

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
    modifySpecial(dailySpecials, 1, "Shrimp Alfredo");  // Correcting a dish name

    cout << endl << "Updated Specials:" << endl;
    displayDailySpecials(dailySpecials);

    // Adding a new special
    addSpecial(dailySpecials, "Chocolate Lava Cake");

    cout << endl << "Final Specials:" << endl;
    displayDailySpecials(dailySpecials);

    return 0;
}