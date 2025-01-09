#include <bits/stdc++.h>
using namespace std;

// structure for information of each cell
struct cell {
    int x, y;
    int distance;
    cell(int x, int y, int distance)
        : x(x), y(y), distance(distance) { }
};

// Utility method for comparing two cells
bool operator<(const cell& a, const cell& b) {
    if (a.distance == b.distance) {
        if (a.x != b.x)
            return (a.x < b.x);
        else
            return (a.y < b.y);
    }
    return (a.distance < b.distance);
}

// Utility method to check whether a point is inside the grid or not
bool isInsideGrid(int i, int j, int row, int col) {
    return (i >= 0 && i < row && j >= 0 && j < col);
}

// Method returns minimum cost to reach bottom-right from top-left
int shortest(const vector<vector<int>>& grid, int row, int col) {
    vector<vector<int>> dis(row, vector<int>(col, INT_MAX));

    // direction arrays for simplification of getting neighbour
    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    set<cell> st;

    // insert (0, 0) cell with 0 distance
    st.insert(cell(0, 0, 0));

    // initialize distance of (0, 0) with its grid value
    dis[0][0] = grid[0][0];

    // loop for standard dijkstra's algorithm
    while (!st.empty()) {
        // get the cell with minimum distance and delete it from the set
        cell k = *st.begin();
        st.erase(st.begin());

        // looping through all neighbours
        for (int i = 0; i < 4; i++) {
            int x = k.x + dx[i];
            int y = k.y + dy[i];

            // if not inside boundary, ignore them
            if (!isInsideGrid(x, y, row, col))
                continue;

            // If distance from current cell is smaller, update distance of neighbour cell
            if (dis[x][y] > dis[k.x][k.y] + grid[x][y]) {
                // If cell is already there in set, then remove its previous entry
                if (dis[x][y] != INT_MAX)
                    st.erase(st.find(cell(x, y, dis[x][y])));

                // update the distance and insert new updated cell in set
                dis[x][y] = dis[k.x][k.y] + grid[x][y];
                st.insert(cell(x, y, dis[x][y]));
            }
        }
    }

    // dis[row - 1][col - 1] will represent final distance of bottom right cell from top left cell
    return dis[row - 1][col - 1];
}

// Driver code to test above methods
int main() {
    int row, col;

    // Get the dimensions of the grid from the user
    cout << "Enter the number of rows: ";
    cin >> row;
    cout << "Enter the number of columns: ";
    cin >> col;

    // Create a dynamic 2D vector for the grid
    vector<vector<int>> grid(row, vector<int>(col));

    // Get the grid values from the user
    cout << "Enter the grid values:" << endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cin >> grid[i][j];
        }
    }

    // Call the shortest function to find the least cost path
    cout << "The minimum cost to reach bottom-right from top-left is: " << shortest(grid, row, col) << endl;

    return 0;
}
