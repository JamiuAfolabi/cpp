#include <iostream>

constexpr int choose_constant(int x) {
    if constexpr (x == 0) {
        return 10;
    } else if constexpr (x == 1) {
        return 20;
    } else {
        return 30;
    }
}

int main() {
    constexpr int value = 1;
    const int result = choose_constant(value);
    std::cout << "Result: " << result << std::endl;
    return 0;
}