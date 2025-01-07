#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

int largestRectangleArea(const vector<int>& heights) {
    stack<int> indices; // Stack to store indices of the bars
    int maxArea = 0; // Variable to store the maximum area
    int n = heights.size();

    for (int i = 0; i < n; ++i) {
        // While the current height is less than the height of the bar at the index on the top of the stack
        while (!indices.empty() && heights[i] < heights[indices.top()]) {
            int height = heights[indices.top()]; // Height of the bar to calculate area
            indices.pop(); // Remove the index of the height being processed
            
            // Determine the width
            int width;
            if (indices.empty()) {
                width = i; // If stack is empty, width is i (from 0 to i)
            } else {
                width = i - indices.top() - 1; // Width is the distance from the current index to the index on the top of the stack
            }
            maxArea = max(maxArea, height * width); // Update max area
        }
        indices.push(i); // Push current index onto stack
    }

    // Process remaining bars in the stack
    while (!indices.empty()) {
        int height = heights[indices.top()];
        indices.pop();

        int width;
        if (indices.empty()) {
            width = n; // Width is n (from 0 to n)
        } else {
            width = n - indices.top() - 1; // Width is the distance from the current index to the index on the top of the stack
        }
        maxArea = max(maxArea, height * width); // Update max area
    }

    return maxArea; // Return the maximum area found
}

int main() {
    vector<int> heights = {6, 2, 5, 4, 5, 1, 6};
    int maxArea = largestRectangleArea(heights);
    cout << "The largest rectangular area in the histogram is: " << maxArea << endl; // Output should be 12
    return 0;
}