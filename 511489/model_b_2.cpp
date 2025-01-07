#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    int left[n], right[n], maxArea = 0, area = 0;

    // Initialize left and right arrays to -1
    fill_n(left, n, -1);
    fill_n(right, n, -1);

    stack<int> s;
    s.push(0);
    for (int i = 1; i < n; i++) {
        while (!s.empty() && heights[i] < heights[s.top()]) {
            int h = s.top();
            s.pop();
            if (s.empty()) {
                left[h] = 0;
            } else {
                left[h] = s.top() + 1;
            }
            area = heights[h] * (left[h]);
            maxArea = max(maxArea, area);
        }
        s.push(i);
    }

    // Process the right half of the bars
    s.push(n - 1);
    for (int i = n - 2; i >= 0; i--) {
        while (!s.empty() && heights[i] < heights[s.top()]) {
            int h = s.top();
            s.pop();
            if (s.empty()) {
                right[h] = n - 1;
            } else {
                right[h] = s.top() - 1;
            }
            area = heights[h] * (right[h] - left[h] + 1);
            maxArea = max(maxArea, area);
        }
        s.push(i);
    }
 
    // For the first and last bars, the area can be calculated directly 
    if (n > 0) {
        maxArea = max(maxArea, heights[0] * (right[0] - left[0] + 1));
    }

    if (n > 1) {
        maxArea = max(maxArea, heights[n - 1] * (right[n - 1] - left[n - 1] + 1));
    }

    return maxArea;
}

int main() {
    vector<int> heights = {6, 2, 5, 4, 5, 1, 6};
    int maxArea = largestRectangleArea(heights);
    cout << "The largest rectangular area in the histogram is: " << maxArea << endl; // Output should be 12
    return 0;
}
