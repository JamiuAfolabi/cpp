#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

struct TrafficSignal {
    int state; // 0 for red, 1 for green
    int duration; // Duration the signal has been in the current state (in seconds)
    TrafficSignal* up;
    TrafficSignal* down;
    TrafficSignal* left;
    TrafficSignal* right;

    TrafficSignal(int s) : state(s), duration(0), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

class TrafficGrid {
private:
    int rows;
    int cols;
    std::vector<std::vector<TrafficSignal*>> grid;

    void checkCongestion(int currentTime) {
        // Only check congestion every 30 seconds
        if (currentTime % 30 != 0) return;
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j]->state == 0 && grid[i][j]->duration > 30) { // Check those which are red and in state for over 30s
                    int count = 0; // Only count adjacent signals, not the current one

                    // Check adjacent signals
                    TrafficSignal* neighbors[4] = {grid[i][j]->up, grid[i][j]->down, grid[i][j]->left, grid[i][j]->right};

                    for (TrafficSignal* neighbor : neighbors) {
                        if (neighbor && neighbor->state == 0 && neighbor->duration > 30) {
                            count++;
                        }
                    }

                    // Detect congestion
                    if (count >= 3) {
                        std::cout << "Congestion detected at signal [" << i << "][" << j << "]! (" << count << " adjacent red signals)\n";
                    }
                }
            }
        }
    }

public:
    TrafficGrid(int r, int c) : rows(r), cols(c) {
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
                if (i > 0) current->up = grid[i - 1][j];
                if (i < rows - 1) current->down = grid[i + 1][j];
                if (j > 0) current->left = grid[i][j - 1];
                if (j < cols - 1) current->right = grid[i][j + 1];
            }
        }
    }

    ~TrafficGrid() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                delete grid[i][j];
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

    void updateSignals(int currentTime) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // Increment the duration for all signals
                if (grid[i][j]->state == 0) { // If the signal is red
                    grid[i][j]->duration++;
                } else { // Reset duration if the signal is green
                    grid[i][j]->duration = 0;
                }
            }
        }

        checkCongestion(currentTime); // Check for congestion after updating
    }

    void setSignalState(int row, int col, int state) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            grid[row][col]->state = state;
            if (state == 1) {
                grid[row][col]->duration = 0; // Reset duration if the signal is changed to green
            }
        }
    }
};

int main() {
    TrafficGrid trafficGrid(5, 5); // Create a 5x5 grid of traffic signals

    // Simulate signals changing over time
    trafficGrid.setSignalState(1, 1, 0); // Set signal at [1, 1] to red
    trafficGrid.setSignalState(1, 2, 0); // Set signal at [1, 2] to red
    trafficGrid.setSignalState(1, 3, 0); // Set signal at [1, 3] to red

    // Simulate updating the grid every second
    for (int cyc = 0; cyc < 120; ++cyc) { // Run for 120 seconds
        trafficGrid.updateSignals(cyc);
        
        if (cyc % 30 == 0) { // Print every 30 seconds
            std::cout << "Time: " << cyc << " seconds\n";
            trafficGrid.printStates();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate 1 second passing
    }

    return 0;
}
