#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int largestRectangleArea(const vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;

    // Dynamically allocate memory for left and right arrays.
    vector<int> left(n, -1);
    vector<int> right(n, n);

    // Pointers for filling the left array
    for (int i = 0; i < n; ++i) {
        const int* current_height = &heights[i];
        int* j = &left[i];

        *j = i - 1;  // Initialize to the index to the left
        while (*j >= 0 && heights[*j] >= *current_height) {
            *j = left[*j]; // Jump to previous smaller element's index
        }
    }

    // Pointers for filling the right array
    for (int i = n - 1; i >= 0; --i) {
        const int* current_height = &heights[i];
        int* j = &right[i];

        *j = i + 1;  // Initialize to the index to the right
        while (*j < n && heights[*j] >= *current_height) {
            *j = right[*j]; // Jump to next smaller element's index
        }
    }

    // Calculate the maximum area
    int maxArea = 0;
    for (int i = 0; i < n; ++i) {
        int width = right[i] - left[i] - 1; // Calculate width
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