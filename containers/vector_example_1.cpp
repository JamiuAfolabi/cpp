#include <iostream>
#include <vector>
using namespace std;

template <typename T> void printVector(const vector<T>& integers2);

int main(){
    vector<int> integers;

    cout << "\ninitial size of integer is: " << integers.size()
    << "\nThe initial capacity of integer is: " << integers.capacity();

    integers.push_back(2);
    integers.push_back(3);
    integers.push_back(4);

    cout << "\nFinal size of integer is: " << integers.size()
    << "\nThe Final capacity of integer is: " << integers.capacity();

    cout << endl;

    const size_t SIZE{6};

    int values[SIZE]{1,2,3,4,5,6};

    for (const int* ptr = cbegin(values); ptr != cend(values); ++ptr){
        cout << *ptr << ' ';
    }

    cout << "\nOutput vectors using iterator notation: ";
    printVector(integers);
    cout << "\nReversed contents of vector integers: ";

    for (auto reverseIterator = integers.crbegin(); reverseIterator != integers.crend(); ++reverseIterator){
        cout << *reverseIterator << ' ';

    }

    cout << endl;

}

template <typename T> void printVector(const vector<T>& integers2){
    for (auto constIterator = integers2.cbegin();
    constIterator != integers2.cend(); ++constIterator){
        cout << *constIterator << ' ';
    }
}


