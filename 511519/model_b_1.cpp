#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> originalArray = {1, 2, 3, 4, 5};

    //printing the original array
    cout << "Original Array: ";
    for (int value : originalArray) {
        cout << value << " ";
    }
    cout << endl;

    // deep copy the originalArray using copy constructor
    vector<int> clonedArray = originalArray;
 
   // modify the clonedArray
    clonedArray[2] = 10;
    clonedArray.push_back(6);

   // printing the cloned array after modification
    cout << "Modified Cloned Array: ";
    for (int value : clonedArray) {
        cout << value << " ";
    }
    cout << endl;

    // printing the original array to verify it remains unchanged
    cout << "Original Array (Remains Unchanged): ";
    for (int value : originalArray) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}
