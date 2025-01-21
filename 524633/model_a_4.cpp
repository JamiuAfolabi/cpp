#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <sys/stat.h> // For directory handling on Linux
#include <sys/types.h>
#include <cerrno>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <direct.h> // For _mkdir
#else
#include <unistd.h>
#endif

const int MAX_FAILED_ATTEMPTS = 3;
const std::string FAILED_LOGIN_LOG = "failed_login_attempts.log";
const std::string DIRECTORY_PATH = "./logs"; // Log directory path

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

// Function to ensure the log directory exists
bool ensureDirectoryExists(const std::string& directory) {
#if defined(_WIN32) || defined(_WIN64)
    if (_mkdir(directory.c_str()) == 0) {
        return true; // Directory was created
    } else if (errno == EEXIST) {
        return true; // Directory already exists
    } else {
        std::cerr << "Failed to create directory: " << directory << " - " << strerror(errno) << std::endl;
        return false;
    }
#else
    struct stat info;
    if (stat(directory.c_str(), &info) != 0) { // Directory does not exist
        if (mkdir(directory.c_str(), 0777) == 0) {
            return true; // Directory was created
        } else {
            std::cerr << "Failed to create directory: " << directory << " - " << strerror(errno) << std::endl;
            return false;
        }
    } else if (info.st_mode & S_IFDIR) { // Directory exists
        return true;
    } else {
        std::cerr << "Path exists but is not a directory: " << directory << std::endl;
        return false;
    }
#endif
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
    // Ensure the log directory exists
    if (!ensureDirectoryExists(DIRECTORY_PATH)) {
        std::cerr << "Error: Unable to proceed without the log directory." << std::endl;
        return EXIT_FAILURE; // Exit if directory cannot be created
    }

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