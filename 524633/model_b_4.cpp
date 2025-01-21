#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <filesystem> // Include this header for std::filesystem tools

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#endif

const int MAX_FAILED_ATTEMPTS = 3;
const std::string FAILED_LOGIN_LOG = "failed_login_attempts.log";
const std::string AUDIT_LOG_DIR = "./restricted_logs";

std::map<std::string, int> failedAttempts;

// Function to log failed login attempts
void logFailedAttempt(const std::string& username) {
    std::ofstream logFile(FAILED_LOGIN_LOG, std::ios::app);
    if (logFile) {
        std::time_t now = std::time(nullptr);
        logFile << "Failed login attempt for user: " << username << " at " << std::ctime(&now) << std::endl;
        logFile.close();
    } else {
        std::cerr << "Error opening failed login attempts log file." << std::endl;
    }
}

// Check if the directory exists. Create it if not, with error logging
void ensureDirectoryExists(const std::string& directory) {
#if defined(_WIN32) || defined(_WIN64)
    // Windows implementation
    DWORD attributes = GetFileAttributesA(directory.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        // The directory does not exist, so try to create it
        if (CreateDirectoryA(directory.c_str(), nullptr) == 0) {
            DWORD err = GetLastError();
            std::cerr << "Error creating directory: " << directory << std::endl;
            std::cerr << "Error Code: " << err << " - " << strerror(err) << std::endl;
        }
    } 
#else
    // POSIX (Linux) implementation
    struct stat sb;
    if (stat(directory.c_str(), &sb) != 0) {
        // Directory doesn't exist
        if (mkdir(directory.c_str(), 0755) != 0) {
            perror("Error creating directory");
        }
    }
#endif
}

// Simulated login function
bool login(const std::string& username, const std::string& password) {
    // Simulated correct credentials
    const std::string correctPassword = "password123";
    // Ensure the necessary logs directory exists
    ensureDirectoryExists(AUDIT_LOG_DIR);

    if (failedAttempts[username] >= MAX_FAILED_ATTEMPTS) {
        std::cout << "User " << username << " is locked out due to too many failed attempts." << std::endl;
        return false;
    }

    if (password == correctPassword) {
        std::cout << "Login successful for user: " << username << std::endl;
        failedAttempts[username] = 0;
        return true;
    } else {
        std::cout << "Invalid credentials for user: " << username << std::endl;
        failedAttempts[username]++;
        logFailedAttempt(username);
        
        if (failedAttempts[username] >= MAX_FAILED_ATTEMPTS) {
            std::cout << "User " << username << " has been locked out due to too many failed attempts." << std::endl;
        }
        return false;
    }
}

int main() {
    std::string username;
    std::string password;

    ensureDirectoryExists(FAILED_LOGIN_LOG);  // Preemptively create failed attempts log directory

    // Simulated login attempts
    for (int attempts = 0; attempts < 5; ++attempts) {
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        failedAttempts[username] = 0; // Populate status at start
        login(username, password);
    }

    return 0;
}