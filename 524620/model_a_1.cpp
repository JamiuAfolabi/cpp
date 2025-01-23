#include <iostream>
#include <cstring> // for strcpy and strlen
#include <vector>
#include <string>

using namespace std;

const int MAX_HEADLINES = 10;
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
    char headlines[MAX_HEADLINES][MAX_LENGTH + 1]; // +1 for null terminator
    int count = 0;

    while (count < MAX_HEADLINES) {
        cout << "Enter headline (or type 'exit' to finish): ";
        cin.getline(headlines[count], MAX_LENGTH + 1); // Get the input

        // Check for exit condition
        if (strcmp(headlines[count], "exit") == 0) {
            break;
        }

        // Validate headline length
        if (strlen(headlines[count]) > MAX_LENGTH) {
            cout << "Headline exceeds maximum length of 100 characters." << endl;
        } else {
            // Categorize and display the headline
            string category = categorizeHeadline(headlines[count]);
            cout << "Category: " << category << endl;
            count++;
        }
    }

    cout << "Entered Headlines:" << endl;
    for (int i = 0; i < count; i++) {
        cout << headlines[i] << endl;
    }

    return 0;
}