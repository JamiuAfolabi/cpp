#include <iostream>
#include <functional>
using namespace std;

// Function to find the reverse of a number in binary
int findReverse(int n) {
    int reverse_bin = 0;
    int temp = n;
    while (temp > 0) {
        reverse_bin = (reverse_bin << 1) | (temp & 1);
        temp = temp >> 1;
    }
    return reverse_bin;
}

// Function to check if the binary representation of the number is a palindrome
bool isPalindrome(int n) {
    return n == findReverse(n);
}

int main() {
    int n;

    // Taking user input
    cout << "Enter a number: ";
    cin >> n;

    // Using a functional approach to print the result
    function<void()> checkPalindrome = [&]() {
        cout << "Is binary representation of " << n << " a palindrome? " 
             << (isPalindrome(n) ? "Yes" : "No") << endl;
    };

    checkPalindrome();  // Calling the function

    return 0;
}
