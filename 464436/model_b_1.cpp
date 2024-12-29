#include <iostream>
// Code with implicit fallthrough (behaviour differs in C++98 and C++03)
int main() {
    int value = 3;
    switch (value) {
        case 1:
            std::cout << "Case 1\n";
            break;
        case 2:
            std::cout << "Case 2\n";
            // Execution will fall through to default in C++98
        default:
            std::cout << "Default case\n";
            break;
    }
    return 0;
}