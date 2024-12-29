#include <iostream>
#include <iterator>
using namespace std;

int main() {
    cout << "Enter two integers: ";

    // Create istream_iterator to read input from cin
    istream_iterator<int> inputInt{cin};
    int number1 = *inputInt;  // Read first integer
    ++inputInt;  // Move to the next input
    int number2 = *inputInt;  // Read second integer

    // Calculate the sum
    int sum = number1 + number2;

    // Create ostream_iterator to output the result to cout
    ostream_iterator<int> outputInt{cout};
    cout << "The sum is: ";
    *outputInt = sum;  // Output the sum

    cout << endl;

    return 0;
}
