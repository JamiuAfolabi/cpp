#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <limits>

using namespace std;

// Structure to represent a cell in the grid
struct Cell {
    int x, y;
    int cost;
};

// Template alias for the cell container (can be changed between set and priority_queue)
template <typename T>
using CellContainer = T;

// Define a comparator for priority_queue when using priority_queue
struct CellComparator {
    bool operator()(const Cell& lhs, const Cell& rhs) const {
        return lhs.cost > rhs.cost;
    }
};

// Utility method to check whether a point is inside the grid
bool isInsideGrid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Dijkstra's algorithm using a custom container defined by template alias
template <typename CellCont>
int shortestPath(const vector<vector<int>>& grid, int rows, int cols) {
    // Create a distance vector initialized to infinity
    vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
    dist[0][0] = grid[0][0];

    // Use the template alias to create the cell container
    CellCont container;

    // Push the starting cell into the container
    if constexpr (is_same<CellCont, priority_queue<Cell, vector<Cell>, CellComparator>>::value) {
        // For priority_queue
        container.push({0, 0, grid[0][0]});
    } else if constexpr (is_same<CellCont, set<Cell>>::value) {
        // For set
        container.insert({0, 0, grid[0][0]});
    }

    // Directions for moving up, right, down, and left
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!container.empty()) {
        Cell current = container.top();
        if constexpr (is_same<CellCont, priority_queue<Cell, vector<Cell>, CellComparator>>::value) {
            // For priority_queue
            container.pop();
        } else if constexpr (is_same<CellCont, set<Cell>>::value) {
            // For set
            container.erase(container.find(current));
        }

        if (current.x == rows - 1 && current.y == cols - 1) {
            return current.cost;
        }

        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isInsideGrid(newX, newY, rows, cols)) {
                int newCost = current.cost + grid[newX][newY];
                if (newCost < dist[newX][newY]) {
                    dist[newX][newY] = newCost;

                    Cell newCell {newX, newY, newCost};
                    if constexpr (is_same<CellCont, priority_queue<Cell, vector<Cell>, CellComparator>>::value) {
                        // For priority_queue
                        container.push(newCell);
                    } else if constexpr (is_same<CellCont, set<Cell>>::value) {
                        // For set
                        container.insert(newCell);
                    }
                }
            }
        }
    }

    return -1;
}

// Driver code to test above methods
int main() {
    int rows, cols;

    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;

    vector<vector<int>> grid(rows, vector<int>(cols));
    cout << "Enter the grid values:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> grid[i][j];
        }
    }

    // Using set for cell container
    int minCostWithSet = shortestPath<set<Cell>>(grid, rows, cols);
    if (minCostWithSet != -1) {
        cout << "Using set: Minimum cost to reach bottom-right from top-left is: " << minCostWithSet << endl;
    } else {
        cout << "Destination is not reachable with set." << endl;
    }

    // Using priority_queue for cell container
    int minCostWithPriorityQueue = shortestPath<priority_queue<Cell, vector<Cell>, CellComparator>>(grid, rows, cols);
    if (minCostWithPriorityQueue != -1) {
        cout << "Using priority_queue: Minimum cost to reach bottom-right from top-left is: " << minCostWithPriorityQueue << endl;
    } else {
        cout << "Destination is not reachable with priority_queue." << endl;
    }

    return 0;
}