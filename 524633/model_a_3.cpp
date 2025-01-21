#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

const int MAX_FAILED_ATTEMPTS = 3;
const std::string FAILED_LOGIN_LOG = "failed_login_attempts.log";

// Data structure to track user login attempts
std::map<std::string, int> failedAttempts;

// Function to log failed login attempts
void logFailedAttempt(const std::string& username) {
    std::ofstream logFile(FAILED_LOGIN_LOG, std::ios::app);
    if (logFile) {
        std::time_t now = std::time(nullptr);
        logFile << "Failed login attempt for user: " << username
                << " at " << std::ctime(&now); // Use ctime for a readable timestamp
        logFile.close();
    } else {
        std::cerr << "Error opening failed login attempts log file." << std::endl;
    }
}

// Function to simulate user login
bool login(const std::string& username, const std::string& password) {
    // Simulated correct credentials for demonstration purposes
    const std::string correctPassword = "password123";

    // Check if the user has exceeded the maximum allowed failed attempts
    if (failedAttempts[username] >= MAX_FAILED_ATTEMPTS) {
        std::cout << "User " << username << " is locked out due to too many failed attempts." << std::endl;
        return false;
    }

    // Simulate checking credentials (for demonstration)
    if (password == correctPassword) {
        std::cout << "Login successful for user: " << username << std::endl;
        failedAttempts[username] = 0; // Reset on successful login
        return true;
    } else {
        std::cout << "Invalid credentials for user: " << username << std::endl;
        failedAttempts[username]++;
        logFailedAttempt(username); // Log this failed attempt

        if (failedAttempts[username] >= MAX_FAILED_ATTEMPTS) {
            std::cout << "User " << username << " has been locked out due to too many failed attempts." << std::endl;
        }
        return false;
    }
}

int main() {
    std::string username;
    std::string password;

    // Simulated login attempts
    for (int attempts = 0; attempts < 5; ++attempts) {
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        login(username, password);
    }

    return 0;
}