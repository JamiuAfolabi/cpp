
#include <iostream>
#include <cstring> // for strcpy and strlen
#include <set>
#include <string>

using namespace std;

const int MAX_HEADLINES = 100;
const int MAX_LENGTH = 100;

// Function to categorize headlines
string categorizeHeadline(const char* headline) {
    if (strstr(headline, "Election") || strstr(headline, "Vote") || strstr(headline, "Government")) {
        return "Politics";
    } else if (strstr(headline, "Game") || strstr(headline, "Score") || strstr(headline, "Team")) {
        return "Sports";
    } else if (strstr(headline, "Film") || strstr(headline, "Music") || strstr(headline, "Celebrity")) {
        return "Entertainment";
    } else if (strstr(headline, "Technology") || strstr(headline, "Gadget") || strstr(headline, "AI")) {
        return "Technology";
    } else {
        return "Uncategorized";
    }
}

int main() {
    set<string> uniqueHeadlines;

    while (true) {
        char headline[MAX_LENGTH + 1]; // +1 for null terminator
        cout << "Enter headline (or type 'exit' to finish): ";
        cin.getline(headline, MAX_LENGTH + 1); // Get the input

        // Check for exit condition
        if (strcmp(headline, "exit") == 0) {
            break;
        }

        // Validate headline length
        if (strlen(headline) > MAX_LENGTH) {
            cout << "Headline exceeds maximum length of 100 characters." << endl;
        } else {
            // Convert the headline to string and check for duplicates
            string strHeadline(headline);
            if (uniqueHeadlines.find(strHeadline) == uniqueHeadlines.end()) {
                uniqueHeadlines.insert(strHeadline);
                
                // Categorize and display the headline
                string category = categorizeHeadline(headline);
                cout << "Category: " << category << endl;
            } else {
                cout << "Duplicate headline found. Skipping." << endl;
            }
        }
    }

    cout << "\nUnique Entered Headlines:" << endl;
    for (const string& headline : uniqueHeadlines) {
        cout << headline << endl;
    }

    return 0;
}
