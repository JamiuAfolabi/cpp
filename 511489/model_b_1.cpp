#include<bits/stdc++.h>
using namespace std;
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    int max_area = 0;

    /* Consider each bar as the left bar of the rectangle and
       calculate the maximum area possible with that bar as left bar */
    for (int i = 0; i < n; i++)
    {
        /* Initialize the height of the rectangle as the height of the current bar */
        int current_height = heights[i];
        int area = 0;

        /* Find the maximum area with the current bar as the left bar and 
           all bars from right side of the current bar */
        for (int j = i; j < n; j++)
        {
            current_height = min(current_height, heights[j]);
            area = max(area, current_height * (j - i + 1));
        }
        max_area = max(max_area, area);
    }

    return max_area;
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> heights = {6, 2, 5, 4, 5, 1, 6};
    int result = largestRectangleArea(heights);
    cout << "Largest rectangular area is: " << result << endl;
    return 0;
}