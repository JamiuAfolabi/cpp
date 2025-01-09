#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <limits>
using namespace std;

// Structure to represent a cell in the grid
struct Cell {
    int x, y;
    int cost;

    // Overloading operator for priority queue; lower cost has higher priority
    bool operator>(const Cell& other) const {
        return cost > other.cost;
    }
};

// Utility method to check whether a point is inside the grid
bool isInsideGrid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Method returns minimum cost to reach bottom-right from top-left
int shortest(const vector<vector<int>>& grid, int rows, int cols) {
    // Create a distance vector initialized to infinity
    vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
    dist[0][0] = grid[0][0];

    // Create a min-heap priority queue
    priority_queue<Cell, vector<Cell>, greater<Cell>> pq;
    // Push the starting cell into the queue
    pq.push({0, 0, grid[0][0]});

    // Directions for moving up, right, down, left
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    // Dijkstra's algorithm implementation
    while (!pq.empty()) {
        Cell current = pq.top();
        pq.pop();

        // If we reached the bottom-right corner, return its cost
        if (current.x == rows - 1 && current.y == cols - 1) {
            return current.cost;
        }

        // Explore the neighbors
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // If the neighbor is within grid boundaries
            if (isInsideGrid(newX, newY, rows, cols)) {
                // Calculate the new cost to reach this neighbor
                int newCost = current.cost + grid[newX][newY];

                // If the new cost is less than the recorded cost, update and push into the priority queue
                if (newCost < dist[newX][newY]) {
                    dist[newX][newY] = newCost;
                    pq.push({newX, newY, newCost});
                }
            }
        }
    }

    // Should not reach here in valid cases; this returns an indication of failure
    return -1;  
}

// Driver code to test above methods
int main() {
    int rows, cols;

    // Get the dimensions of the grid from the user
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;

    // Create a dynamic 2D vector for the grid
    vector<vector<int>> grid(rows, vector<int>(cols));

    // Get the grid values from the user
    cout << "Enter the grid values:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> grid[i][j];
        }
    }

    // Call the shortest function to find the least cost path
    int minCost = shortest(grid, rows, cols);
    if (minCost != -1) {
        cout << "The minimum cost to reach bottom-right from top-left is: " << minCost << endl;
    } else {
        cout << "Destination is not reachable." << endl;
    }

    return 0;
}