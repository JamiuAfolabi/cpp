#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int lengthOfLIS(const vector<int>& arr) {
    if (arr.empty()) return 0;

    vector<int> dp(arr.size(), 1); // Create a DP array and initialize it to 1

    for (size_t i = 1; i < arr.size(); i++) {
        for (size_t j = 0; j < i; j++) {
            if (arr[i] > arr[j]) {
                dp[i] = max(dp[i], dp[j] + 1); // Update the DP array
            }
        }
    }

    // The length of the longest increasing subsequence will be the maximum value in dp array
    return *max_element(dp.begin(), dp.end());
}

int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    int lengthLIS = lengthOfLIS(arr);
    cout << "Length of Longest Increasing Subsequence: " << lengthLIS << endl;
    return 0;
}