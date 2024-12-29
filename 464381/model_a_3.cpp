#include <iostream>

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int x = 5, y = 10;
    std::cout << "The maximum of " << x << " and " << y << " is " << max(x, y) << std::endl;
    return 0;
}