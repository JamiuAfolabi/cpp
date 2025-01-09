#include<bits/stdc++.h>
using namespace std;

int minCostPath(int grid[][], int rows, int cols)
{
    // Base case: If the grid is empty or has only one cell
    if (rows == 0 || cols == 0) {
        return 0;
    }

    // Initialize a 2D array to store the minimum costs required to reach each cell
    int dp[rows][cols];

    // The cost to reach the top-left cell is the cost of that cell itself
    dp[0][0] = grid[0][0];

    // Initialize the first row and column of the dp table
    for (int i = 1; i < cols; i++) {
        dp[0][i] = dp[0][i - 1] + grid[0][i];
    }

    for (int i = 1; i < rows; i++) {
        dp[i][0] = dp[i - 1][0] + grid[i][0];
    }

    // Calculate the minimum cost for each cell in the rest of the grid
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }

    // Return the minimum cost required to reach the bottom-right cell
    return dp[rows - 1][cols - 1];
}
 
int main() {
    int rows, cols;

    cout << "Enter the number of rows in the grid: ";
    cin >> rows;

    cout << "Enter the number of columns in the grid: ";
    cin >> cols;

    // Create a 2D array to store the grid costs
    int grid[rows][cols];

    cout << "Enter the cost of each cell separated by space (row by row): ";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> grid[i][j];
        }
    }

    int minCost = minCostPath(grid, rows, cols);

    cout << "Minimum cost to reach the bottom-right cell from the top-left cell is: " << minCost << endl;

    return 0;
}