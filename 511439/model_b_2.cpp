#include <iostream>
#include <vector>
using namespace std;

int lengthOfLIS(vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return 0;
    
    vector<int> end(n);
    end[0] = arr[0];
    int len = 1;
    
    for (int i = 1; i < n; i++) {
        int low = 0, high = len - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (end[mid] < arr[i]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        if (low == len) {
            end.push_back(arr[i]);
            len++;
        } else {
            end[low] = arr[i];
        }
    }
    return len;
}
 
int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    int lengthLIS = lengthOfLIS(arr);
    cout << "Length of Longest Increasing Subsequence: " << lengthLIS << endl;
    return 0;
}