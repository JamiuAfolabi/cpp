
#include<iostream>
using namespace std;
 
int main()
{
    const int numItems = 4;
    char* menuItems[numItems] = {
        "Pizza",
        "Cheese Burger",
        "Spagetti",   // Note the typo "Spagetti"
        "French Fries"
    };

    cout << "Original Menu:\n";
    for (int i = 0; i < numItems; i++) {
        cout << i+1 << ". " << menuItems[i] << endl;
    }

    // Modifying item name to fix the typo
    strcpy(menuItems[2], "Spaghetti"); // Corrected spelling

    cout << "\nMenu after fixing typo:\n";
    for (int i = 0; i < numItems; i++) {
        cout << i+1 << ". " << menuItems[i] << endl;
    }
    return 0;
}
