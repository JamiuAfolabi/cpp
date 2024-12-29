#include <iostream>
using namespace std;

// Inline function to find the maximum of two numbers
inline int max(int a, int b) {
    return a >= b ? a : b;
}

int main() {
    int a = 10, b = 20, c;
    c = max(a, b);
    cout << "Maximum of " << a << " and " << b << " is: " << c << endl;
    return 0;
}