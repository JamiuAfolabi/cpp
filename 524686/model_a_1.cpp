#include <iostream>
#include <cstring>  // For C-style string functions

using namespace std;

// Function to display the menu items
void displayMenu(const char menu[][30], int size) {
    cout << "Menu Items:" << endl;
    for (int i = 0; i < size; ++i) {
        cout << i + 1 << ". " << menu[i] << endl;
    }
}

// Function to modify an item name
void modifyMenuItem(char menu[][30], int index, const char* newName) {
    strcpy(menu[index], newName);  // Copy the new name into the specified index
}

int main() {
    // Define the maximum number of menu items
    const int MAX_ITEMS = 5;
    // Array to store menu items; assuming max length of each item is 30 characters
    char menu[MAX_ITEMS][30] = {
        "Burger",
        "Spagetti",   // Intentionally misspelled
        "Salad",
        "Pizza",
        "Ice Cream"
    };

    // Display the original menu
    displayMenu(menu, MAX_ITEMS);

    // Correcting the typo from "Spagetti" to "Spaghetti"
    const char* correctName = "Spaghetti";
    modifyMenuItem(menu, 1, correctName);  // Index 1 corresponds to "Spagetti"

    cout << endl << "Updated Menu:" << endl;
    // Display the updated menu
    displayMenu(menu, MAX_ITEMS);

    return 0;
}