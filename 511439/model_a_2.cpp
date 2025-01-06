#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int lengthOfLIS(const vector<int>& arr) {
    if (arr.empty()) return 0;

    vector<int> dp; // This will hold the smallest tail element of each increasing subsequence

    for (int num : arr) {
        // Use binary search to find the location to insert num in the dp array
        auto it = lower_bound(dp.begin(), dp.end(), num);
        if (it == dp.end()) {
            // If num is greater than all elements in dp, append it
            dp.push_back(num);
        } else {
            // Otherwise, replace the found element with num
            *it = num;
        }
    }

    // The length of dp will be the length of the longest increasing subsequence
    return dp.size();
}

int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    int lengthLIS = lengthOfLIS(arr);
    cout << "Length of Longest Increasing Subsequence: " << lengthLIS << endl;
    return 0;
}