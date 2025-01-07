#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int largestRectangleArea(const vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;

    vector<int> left(n, -1);
    vector<int> right(n, n);

    // Fill left array (Previous smaller element's index)
    for (int i = 0; i < n; ++i) {
        int j = i - 1;
        while (j >= 0 && heights[j] >= heights[i]) {
            j = left[j]; // Jump to previous smaller element's index
        }
        left[i] = j; // Assign the index of the previous smaller bar
    }

    // Fill right array (Next smaller element's index)
    for (int i = n - 1; i >= 0; --i) {
        int j = i + 1;
        while (j < n && heights[j] >= heights[i]) {
            j = right[j]; // Jump to next smaller element's index
        }
        right[i] = j; // Assign the index of the next smaller bar
    }

    // Calculate the maximum area
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