#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <algorithm>

using namespace std;

int main(){
    vector<int> values{1,2,3,4,5,6};
    vector<int> integers{values.cbegin(), values.cend()};
    ostream_iterator<int> output{cout, " "};

    cout << "Vector integers contains: ";
    copy(integers.cbegin(), integers.cend(), output);

    cout << "\nFirst element of integers: " << integers.front()
    << "\nLast element of integers: " << integers.back();

    integers[0] = 7;
    integers.at(2) = 10;

    integers.insert(integers.cbegin() + 1, 22);

    cout << "\n\nContents of vector integers after changes: ";
    copy(integers.cbegin(), integers.cend(), output);


    try {
        integers.at(100) = 777;
    }
    catch (out_of_range &outOfRange) {
        cout << "\n\nException: " << outOfRange.what();

        integers.erase(integers.cbegin());

        copy(integers.cbegin(), integers.cend(), output);

        integers.erase(integers.cbegin(), integers.cend());

        cout << "\n\nAfter erasing all ements, vector integers "
        << (integers.empty() ? "is" : "is not ") << "empty" << endl;
        copy(integers.cbegin(), integers.cend(), output);

        integers.clear();

        cout << "\nAfter clear, vector integers "
        << (integers.empty() ? "is" : "is not ") << "empty" << endl;
    }
}

