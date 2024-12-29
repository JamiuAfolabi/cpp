#include <iostream>

template<typename T>
void processValue(T value) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "Integer case: " << value << std::endl;
    } else if constexpr (std::is_floating_point<T>::value) {
        std::cout << "Floating point case: " << value << std::endl;
    } else {
        std::cout << "Default case: Unsupported type" << std::endl;
    }
}

int main() {
    processValue(10);    // Integer case
    processValue(3.14);  // Floating point case
    processValue("text"); // Default case
}