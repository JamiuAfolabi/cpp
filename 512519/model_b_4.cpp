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

template <typename Container>
class DijkstraAlgo {
public:
    
    // Refactored Dijkstra's algorithm with template for interchangeable container types
    int shortest(const vector<vector<int>>& grid, int rows, int cols) {
        // Initialize distance vector
        vector<vector<int>> dist(rows, vector<int>(cols, numeric_limits<int>::max()));
        dist[0][0] = grid[0][0];

        // Container-related operations will be managed by manageContainer functions
        initContainer(grid, pq, rows, cols);

        // Directions for exploring neighbor cells
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        
        while (!pq.empty()) {
            beatCurrentBest(pq, dist, dx, dy, rows, cols);
        }

        return is destination reached ? dist[rows - 1][cols - 1] : -1;
    }

private:
    // Custom defined container
    CellQueue<Container> pq;

    // Function to initialize the cell container with the first cell
    void initContainer(const vector<vector<int>>& grid, CellQueue& pq, int rows, int cols) {
        push into pq({0, 0, grid[0][0]});
    }

    // Function to update the distance of the neighbors and the container 
    void beatCurrentBest(CellQueue& pq, vector<vector<int>>& dist, int dx[], int dy[], int rows, int cols) {
        pop a cell(const Cell&);
        
        if is destination reached { 
            return;
        }
        
        // Touch all four neighbor cells
        for (int i = 0; i < 4; i++) {
            int newX = get moved X(newCell, dx[i]);
            int newY = get moved Y(newCell, dy[i]);

            if is inside grid {
                int newCost = get new cost(newCell, grid, dx[i], dy[i]);
                if is better cost than recorded {
                    update dist(newX, newY, newCost);
                    push into pq({newX, newY, newCost});
                }
            }
        }
    }

 // . ,  Private functions for individual logic  activities
bool is destination reached() const {
        return whatever condition;
    }

    // Other utility functions for said individual logical activity.
    
};

// Driver code to test above methods
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