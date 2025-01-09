#include<bits/stdc++.h>
using namespace std;

// Structure to store cell information
struct cell {
    int x, y;
    int distance;
};

// Comparator function to sort the priority queue by distance (ascending)
bool compare(const cell& a, const cell& b) {
    return (a.distance < b.distance);
}

// Utility method to check if the point is inside the grid or not
bool isInsideGrid(int i, int j, int row, int col) {
    return (i >= 0 && i < row && j >= 0 && j < col);
}
 
int shortest_cost(const vector<vector<int>>& grid, int row, int col) {

    vector<vector<int>> dis(row, vector<int>(col, INT_MAX));
    // Direction arrays to explore neighbor cells
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    // Start with the top-left cell and put it in the priority queue.
    priority_queue<cell, vector<cell>, decltype(compare)> pq({0, 0, grid[0][0]}, compare);
    dis[0][0] = grid[0][0];

    while (!pq.empty()) {
        cell curr = pq.top();
        pq.pop();

        for (int i = 0; i < 4; i++) {
            int x = curr.x + dx[i];
            int y = curr.y + dy[i];

            if (!isInsideGrid(x, y, row, col))
                continue;

            // Calculate the total cost to reach the neighbour cell
            int next_cost = dis[curr.x][curr.y] + grid[x][y];

            // If a shorter path is found, update the distance and add it to the priority queue
            if (next_cost < dis[x][y]) {
                dis[x][y] = next_cost;
                pq.push({x, y, next_cost});
            }
        }
    }

    return dis[row - 1][col - 1];
} 
 
// Driver code
int32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int row, col;
    cout << "Enter number of rows: ";
    cin >> row;
    cout << "Enter number of columns: ";
    cin >> col;
    
    vector<vector<int>> grid(row, vector<int>(col));
    cout << "Enter grid values : \n";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cin >> grid[i][j];
        }
    }
    
    cout << "Minimum cost : " << shortest_cost(grid, row, col) << endl;

    return 0;
}
