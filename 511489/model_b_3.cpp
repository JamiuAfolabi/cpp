#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int largestRectangleArea(const vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;

    vector<int> left(n, -1);
    vector<int> right(n, n);

    // Traverse from left to right and calculate previous smaller index using pointers
    for (int i = 0; i < n; ++i) {
        int* prevPtr = &left[i];
        while (prevPtr >= &left[0] && heights[*prevPtr] >= heights[i]) {
            --prevPtr;
        }
        if (prevPtr >= &left[0])
            *prevPtr = i;
    }

    // Traverse from right to left and calculate next smaller index using pointers
    for (int i = n - 1; i >= 0; --i) {
        int* nextPtr = &right[i];
        while (nextPtr <= &right[n - 1] && heights[*nextPtr] >= heights[i]) {
            ++nextPtr;
        }
        if (nextPtr <= &right[n - 1])
            *nextPtr = i;
    }
 
    // Rest of the code remains the same
    // ...
    int maxArea = 0;
    for (int i = 0; i < n; ++i) {
        int width = right[i] - left[i] - 1; // Width is the distance to the next smaller on the right and previous smaller on the left
        maxArea = max(maxArea, heights[i] * width);
    }

    return maxArea;
}

int main() {
    vector<int> heights = {6, 2, 5, 4, 5, 1, 6};
    int maxArea = largestRectangleArea(heights);
    cout << "The largest rectangular area in the histogram is: " << maxArea << endl; // Output should be 12
    return 0;
}