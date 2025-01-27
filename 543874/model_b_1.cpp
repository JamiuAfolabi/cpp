
#include <iostream>
#include <cstddef>
using namespace std;

enum class SignalState {
    RED,
    GREEN
};

struct TrafficSignal {
    SignalState currentState;
    TrafficSignal* top;
    TrafficSignal* right;
    TrafficSignal* bottom;
    TrafficSignal* left;
};

TrafficSignal** createTrafficSignalGrid(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        cout << "Grid size must be positive." << endl;
        return nullptr;
    }

    TrafficSignal** grid = new TrafficSignal*[rows];

    for (int i = 0; i < rows; ++i) {
        grid[i] = new TrafficSignal[cols];
        for (int j = 0; j < cols; ++j) {
            // Initialize each signal to red and nullptr pointers
            grid[i][j].currentState = SignalState::RED;
            grid[i][j].top = grid[i][j].right = grid[i][j].bottom = grid[i][j].left = nullptr;
        }
    }
    // Connect adjacent signals using pointer arithmetic
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            //Connect left and right
            if (col + 1 < cols) {
                grid[row][col].right = &grid[row][col + 1];
                grid[row][col + 1].left = &grid[row][col];
            }

            // Connect top and bottom
            if (row + 1 < rows) {
                grid[row][col].bottom = &grid[row + 1][col];
                grid[row + 1][col].top = &grid[row][col];
            }
        }
    }

    return grid;
}

void displayGrid(TrafficSignal** grid, int rows, int cols) {
    cout << "Current Traffic Signal States:\n";
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            cout << (grid[row][col].currentState == SignalState::RED ? "R" : "G") << " ";
        }
        cout << endl;
    }
}
 
void deleteTrafficSignalGrid(TrafficSignal** grid, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}
int main() {
    const int rows = 3;
    const int cols = 4;
    TrafficSignal** trafficSignals = createTrafficSignalGrid(rows, cols);

    if (trafficSignals != nullptr) {
        displayGrid(trafficSignals, rows, cols);

        // Example: Modify the state of some signals
        trafficSignals[1][2].currentState = SignalState::GREEN;
        trafficSignals[0][0].currentState = SignalState::GREEN;

        cout << "\nAfter updating signal states:\n";
        displayGrid(trafficSignals, rows, cols);

        deleteTrafficSignalGrid(trafficSignals, rows); // Important to deallocate memory
    }

    return 0;
}
                    
