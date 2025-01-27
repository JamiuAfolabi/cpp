#include <iostream>
#include <cstring>
#include <cstdlib>  // For malloc and free

using namespace std;

const int MAX_SPECIALS = 5;

// Function to display daily specials
void displaySpecials(char** specials, int size) {
    cout << "Daily Specials:" << endl;
    for (int i = 0; i < size; ++i) {
        if (specials[i] != nullptr) {
            cout << i + 1 << ". " << specials[i] << endl;
        }
    }
}

// Function to modify a daily special
void modifyDailySpecial(char** specials, int index, const char* newName) {
    free(specials[index]);  // Free the old memory
    specials[index] = strdup(newName); // Allocate memory and copy new name
}

// Function to add a new daily special
void addDailySpecial(char** specials, int& count, const char* newName) {
    if (count >= MAX_SPECIALS) {
        cout << "Error: Maximum daily specials reached." << endl;
        return;
    }
    specials[count] = strdup(newName);
    count++;
}

// Function to delete a daily special (assume last one)
void deleteDailySpecial(char** specials, int& count) {
    if (count == 0) {
        return;
    }
    free(specials[count - 1]);
    specials[count - 1] = nullptr;
    count--;
}

int main() {
    char** dailySpecials = new char*[MAX_SPECIALS];  // Array of pointers to strings
    int specialCount = 0;

    dailySpecials[0] = strdup("Chicken Tikka Masala");
    dailySpecials[1] = strdup("Lemon Garlic Shrimp");
    specialCount = 2;

    displaySpecials(dailySpecials, specialCount);

    // Updating a daily special
    const char* newSpecial = "Cajun Pasta";
    modifyDailySpecial(dailySpecials, 1, newSpecial);

    // Adding a new daily special
    const char* addSpecial = "Beef Wellington";
    addDailySpecial(dailySpecials, specialCount, addSpecial);

    // Deleting a daily special
    deleteDailySpecial(dailySpecials, specialCount);

    cout << endl << "Updated Daily Specials:" << endl;
    displaySpecials(dailySpecials, specialCount);

    // Remember to clean up the allocated memory
    for (int i = 0; i < specialCount; ++i) {
        free(dailySpecials[i]);
    }
    delete[] dailySpecials;

    return 0;
}