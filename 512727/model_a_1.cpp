#include <iostream>
using namespace std;

// Function to check if the binary representation is a palindrome
bool isBinaryPalindrome(int number) {
    // Edge case for zero
    if (number == 0) return true;
    
    int left = 0;                          // Starting bit position
    int right = sizeof(number) * 8 - 1;  // Ending bit position (assuming 32-bit integer)
    
    while (left < right) {
        // Get the bit at position 'left'
        int leftBit = (number >> left) & 1;
        // Get the bit at position 'right'
        int rightBit = (number >> right) & 1;
        
        // Compare the bits
        if (leftBit != rightBit) {
            return false; // The bits are not the same
        }

        // Move towards the center
        left++;
        right--;
    }
    
    return true; // All bits matched, it's a palindrome
}

int main() {
    int number;
    
    // Input a number from user
    cout << "Enter a number: ";
    cin >> number;

    // Check if its binary representation is a palindrome
    if (isBinaryPalindrome(number)) {
        cout << "The binary representation of " << number << " is a palindrome." << endl;
    } else {
        cout << "The binary representation of " << number << " is not a palindrome." << endl;
    }

    return 0;
}