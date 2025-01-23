#include <iostream>
#include <cstring> // for strcpy and strlen
#include <set>
#include <string>

using namespace std;

const int MAX_LENGTH = 100;

// Function to categorize headlines
string categorizeHeadline(const string& headline) {
    if (headline.find("Election") != string::npos || headline.find("Vote") != string::npos || headline.find("Government") != string::npos) {
        return "Politics";
    } else if (headline.find("Game") != string::npos || headline.find("Score") != string::npos || headline.find("Team") != string::npos) {
        return "Sports";
    } else if (headline.find("Film") != string::npos || headline.find("Music") != string::npos || headline.find("Celebrity") != string::npos) {
        return "Entertainment";
    } else if (headline.find("Technology") != string::npos || headline.find("Gadget") != string::npos || headline.find("AI") != string::npos) {
        return "Technology";
    } else {
        return "Uncategorized";
    }
}

int main() {
    set<string> headlines; // To store unique headlines
    string input;

    while (true) {
        cout << "Enter headline (or type 'exit' to finish): ";
        getline(cin, input); // Get the input

        // Check for exit condition
        if (input == "exit") {
            break;
        }

        // Validate headline length
        if (input.length() > MAX_LENGTH) {
            cout << "Headline exceeds maximum length of 100 characters." << endl;
        } else {
            // Insert into the set (duplicates will be ignored)
            auto result = headlines.insert(input);
            if (result.second) { // Check if the headline was added (true if it was unique)
                string category = categorizeHeadline(input);
                cout << "Category: " << category << endl;
            } else {
                cout << "Duplicate headline, not added." << endl;
            }
        }
    }

    // Display entered unique headlines
    cout << "\nEntered Unique Headlines:" << endl;
    for (const auto& headline : headlines) {
        cout << headline << endl;
    }

    return 0;
}