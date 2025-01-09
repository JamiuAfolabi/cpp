#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <functional>
using namespace std;

// Structure to represent a cell in the grid
struct Cell {
    int x, y;
    int cost;

    // Overloading operator for sorting in set or priority_queue; lower cost has higher priority
    bool operator<(const Cell& other) const {
        return cost < other.cost; // For set: lower cost first
    }

    bool operator>(const Cell& other) const {
        return cost > other.cost; // For priority_queue: lower cost first
    }
};

// Template alias to allow using either std::set or std::priority_queue
template <typename T>
using CellQueue = T;

// Utility method to check whether a point is inside the grid
bool isInsideGrid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Function to manage inserting or pushing cells into the container
template <typename Container>
void enqueue(Container& pq, const Cell& cell) {
    if constexpr (is_same_v<Container, set<Cell>>) {
        pq.insert(cell); // For set: insert
    } else {
        pq.push(cell); // For priority_queue: push
    }
}

// Function to manage extracting the minimum cell from the container
template <typename Container>
Cell dequeue(Container& pq) {
    if constexpr (is_same_v<Container, set<Cell>>) {
        Cell minCell = *pq.begin();
        pq.erase(pq.begin()); 
        return minCell;
    } else {
        Cell minCell = pq.top();
        pq.pop();
        return minCell;
    }
}

// Refactored Dijkstra's algorithm with template for interchangeable container types
template <typename Container>
int shortest(const vector<vector<int>>& grid, int rows, int cols) {
    // Create a distance vector initialized to infinity
    vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
    dist[0][0] = grid[0][0];

    // Create the priority queue or set for managing cells, can be switched easily
    Container pq;

    // Push the starting cell into the queue or set
    enqueue(pq, {0, 0, grid[0][0]});

    // Directions for moving up, right, down, left
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    // Dijkstra's algorithm implementation
    while (!pq.empty()) {
        Cell current = dequeue(pq);

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

                // If the new cost is less than the recorded cost, update and push into the queue or set
                if (newCost < dist[newX][newY]) {
                    dist[newX][newY] = newCost;
                    enqueue(pq, {newX, newY, newCost}); // Push to either set or priority_queue
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

    // Choice of container for Dijkstra's algorithm (set or priority_queue)
    cout << "Choose container type: 1 for set, 2 for priority_queue: ";
    int choice;
    cin >> choice;

    int minCost;
    if (choice == 1) {
        // Using set
        minCost = shortest<set<Cell>>(grid, rows, cols);
    } else {
        // Using priority_queue
        minCost = shortest<priority_queue<Cell, vector<Cell>, greater<Cell>>>(grid, rows, cols);
    }

    if (minCost != -1) {
        cout << "The minimum cost to reach bottom-right from top-left is: " << minCost << endl;
    } else {
        cout << "Destination is not reachable." << endl;
    }

    return 0;
}