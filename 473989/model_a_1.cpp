#include <iostream>
#include <string>

using namespace std;

// Function to direct candies based on their color
void directCandy(const string& color) {
    if (color == "red") {
        cout << "Directing candy to the packaging section." << endl;
    } else if (color == "white") {
        cout << "Directing candy to the inspection section." << endl;
    } else if (color == "blue") {
        cout << "Directing candy to further processing." << endl;
    } else {
        cout << "Color not recognized. Please check the candy color." << endl;
    }
}

int main() {
    string candyColor;
    
    cout << "Enter the color of the candy (red, white, blue): ";
    cin >> candyColor;

    // Convert input to lowercase to ensure uniformity
    for (auto& c : candyColor) {
        c = tolower(c);
    }

    directCandy(candyColor);

    return 0;
}