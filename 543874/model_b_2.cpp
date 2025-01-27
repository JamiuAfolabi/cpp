#include <iostream>
#include <vector>
using namespace std;

const int CONGESTION_THRESHOLD_SIGNALS = 3; // 3 or more adjacent signals for congestion
const int CONGESTION_THRESHOLD_TIME = 30;  // 30 seconds for congestion

struct TrafficSignal {
    int state; // 0 for red, 1 for green
    TrafficSignal* up;
    TrafficSignal* down;
    TrafficSignal* left;
    TrafficSignal* right;
    int duration; // Duration in seconds for the current state
    bool isCongested = false;  // Flag to indicate congestion
    int congestionDuration = 0;  // Duration of congestion

    TrafficSignal(int s) : state(s), duration(0) {}
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
    // Update the duration of each signal state and detect congestion
    void tick() {
        // Reset congestion flag for all signals
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                TrafficSignal* signal = grid[i][j];
                signal->isCongested = false;
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                TrafficSignal* signal = grid[i][j];

                // Increment the duration of the current state
                signal->duration++;

                int redSignalCount = countRedAdjacent(signal);

                // If congestion is detected, set the flags for relevant signals
                if (redSignalCount >= CONGESTION_THRESHOLD_SIGNALS && signal->state == 0) {
                    // Congestion detected! Let's inform all red signals.
                    propagateCongestion(signal);
                }
            }
        }
    }

private:
    int countRedAdjacent(TrafficSignal* signal) const {
        int count = 0;
        if (signal->up && signal->up->state == 0) count++;
        if (signal->down && signal->down->state == 0) count++;
        if (signal->left && signal->left->state == 0) count++;
        if (signal->right && signal->right->state == 0) count++;
        if (signal->state == 0) count++; // Count the current signal
        return count;
    }
     void propagateCongestion(TrafficSignal* signal) {
        signal->isCongested = true;
        signal->congestionDuration = (signal->congestionDuration >= 0) ? signal->congestionDuration + 1 : 1;

        if (signal->up) propagateCongestion(signal->up);
        if (signal->down) propagateCongestion(signal->down);
        if (signal->left) propagateCongestion(signal->left);
        if (signal->right) propagateCongestion(signal->right);
    }

    void displayCongestedSignals() const {
        cout << "\nCongested Signals and their durations:" << endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                TrafficSignal* signal = grid[i][j];
                if (signal->isCongested && signal->congestionDuration >= CONGESTION_THRESHOLD_TIME) {
                    cout << "Signal at row " << i << ", col " << j
                         << " is congested for " << signal->congestionDuration << " seconds" << endl;
                }
            }
        }
    }
};

int main() {
    TrafficGrid trafficGrid(5, 5); 

   // some code.. (Unchanged)

   for (int i = 0; i < 60; ++i) {   // Simulate time passing
        trafficGrid.tick();
        // Change the state of some signals here to illustrate the...
    }
    trafficGrid.displayCongestedSignals(); 
    return 0;
}