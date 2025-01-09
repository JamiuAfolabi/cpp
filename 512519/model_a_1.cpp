#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Function to find the minimum cost path
int minCostPath(const vector<vector<int>>& cost, int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, 0));

    // Initialize the starting cell
    dp[0][0] = cost[0][0];

    // Fill the first row
    for (int j = 1; j < n; j++) {
        dp[0][j] = dp[0][j - 1] + cost[0][j];
    }

    // Fill the first column
    for (int i = 1; i < m; i++) {
        dp[i][0] = dp[i - 1][0] + cost[i][0];
    }

    // Fill the rest of the dp matrix
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + cost[i][j];
        }
    }

    return dp[m - 1][n - 1]; // The bottom-right cell contains the minimum cost
}

int main() {
    int m, n;

    // Input grid dimensions
    cout << "Enter the number of rows (m): ";
    cin >> m;
    cout << "Enter the number of columns (n): ";
    cin >> n;

    vector<vector<int>> cost(m, vector<int>(n));

    // Input the cost grid
    cout << "Enter the cost grid (m x n):" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> cost[i][j];
        }
    }

    // Calculate the minimum cost path
    int minCost = minCostPath(cost, m, n);

    // Output the result
    cout << "The minimum cost to traverse from top-left to bottom-right is: " << minCost << endl;

    return 0;
}