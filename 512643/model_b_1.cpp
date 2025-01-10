#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class MyArray {
private:
    vector<int> elements;

public:
    void insertFront(int element) {
        elements.insert(elements.begin(), element);
    }

    void insertBack(int element) {
        elements.push_back(element);
    }

    int searchElement(int target) {
        size_t index = std::distance(elements.begin(), std::find(elements.begin(), elements.end(), target));
        if (index == elements.size()) {
            return -1;
        }
        return static_cast<int>(index);
    }

    void displayArray() {
        for (const int& element : elements) {
            cout << element << " ";
        }
        cout << endl;
    }
};

int main() {
    MyArray array;

    array.insertFront(50);
    array.insertBack(30);
    array.insertFront(20);
    array.insertBack(40);
    array.insertBack(60);

    array.displayArray(); // Output: 20 50 30 40 60 

    int target = 30;
    int index = array.searchElement(target);
    if (index != -1) {
        cout << "Found " << target << " at index " << index << endl;
    } else {
        cout << "Element " << target << " not found." << endl;
    }

    return 0;
}