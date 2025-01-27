#include <iostream>
#include <vector>

struct TrafficSignal {
    int state; // 0 for red, 1 for green
    TrafficSignal* up;
    TrafficSignal* down;
    TrafficSignal* left;
    TrafficSignal* right;

    TrafficSignal(int s) : state(s), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

class TrafficGrid {
private:
    int rows;
    int cols;
    std::vector<std::vector<TrafficSignal*>> grid;

public:
    TrafficGrid(int r, int c) : rows(r), cols(c) {
        // Create a grid of traffic signals initialized to red (0)
        for (int i = 0; i < rows; ++i) {
            std::vector<TrafficSignal*> row;
            for (int j = 0; j < cols; ++j) {
                row.push_back(new TrafficSignal(0)); // Initial state as red
            }
            grid.push_back(row);
        }

        // Connect signals to their neighbors using pointer arithmetic
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                TrafficSignal* current = grid[i][j];

                // Connect the current signal with its neighbors
                if (i > 0) current->up = grid[i - 1][j];             // Up
                if (i < rows - 1) current->down = grid[i + 1][j];   // Down
                if (j > 0) current->left = grid[i][j - 1];          // Left
                if (j < cols - 1) current->right = grid[i][j + 1];  // Right
            }
        }
    }

    ~TrafficGrid() {
        // Clean up the allocated memory for traffic signals
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                delete grid[i][j]; // Deallocate each signal
            }
        }
    }

    void printStates() const {
        for (const auto& row : grid) {
            for (const auto& signal : row) {
                std::cout << signal->state << " ";
            }
            std::cout << std::endl;
        }
    }

    // Function to set the state of a specific signal
    void setSignalState(int row, int col, int state) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            grid[row][col]->state = state;
        }
    }
};

int main() {
    TrafficGrid trafficGrid(5, 5); // Create a 5x5 grid of traffic signals

    // Set some signals to green (1)
    trafficGrid.setSignalState(1, 1, 1);
    trafficGrid.setSignalState(2, 2, 1);
    trafficGrid.setSignalState(3, 3, 1);

    // Print the states of the traffic signals
    std::cout << "Current states of traffic signals:\n";
    trafficGrid.printStates();

    return 0;
}