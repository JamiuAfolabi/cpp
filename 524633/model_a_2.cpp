#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <map>

// Check for Windows or Linux
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#endif

enum UserType {
    ADMIN,
    DOCTOR,
    NURSE,
    UNKNOWN
};

// Function to get user type based on input (for demonstration purposes)
UserType getUserType(const std::string& username) {
    if (username == "admin") return ADMIN;
    if (username == "doctor") return DOCTOR;
    if (username == "nurse") return NURSE;
    return UNKNOWN;
}

bool hasPermission(UserType userType, const std::string& directory, bool isRestricted) {
    if (isRestricted) {
        if (userType == ADMIN) {
            return true;  // Admin has access to all
        } else if (userType == DOCTOR) {
            std::cout << "Doctor access requested on a restricted directory." << std::endl;
        } else if (userType == NURSE) {
            std::cout << "Nurse access requested on a restricted directory." << std::endl;
        }
        return false; // no access for doctors and nurses
    }
    return true; // Allow all to access non-restricted log directory
}

void logAuditEntry(const std::string& directory, const std::string& entry, UserType userType, bool isRestricted) {
    std::string logFilePath = directory + "/audit_log.txt";

    if (!hasPermission(userType, directory, isRestricted)) {
        std::cerr << "Access denied for user type." << std::endl;
        return;
    }

    std::ofstream logFile;
    logFile.open(logFilePath, std::ios::app);

    // Check if the file opened properly
    if (!logFile) {
        int errorCode = errno;
        std::cerr << "Error opening file: " << logFilePath << std::endl;

#if defined(_WIN32) || defined(_WIN64)
        // Windows specific error reporting
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError(); 

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, 
            dw, 
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
            (LPTSTR)&lpMsgBuf, 
            0, 
            NULL);

        std::cerr << "Error Code: " << dw << " - " << (char*)lpMsgBuf << std::endl;
        LocalFree(lpMsgBuf);
#else
        // POSIX (Linux) specific error reporting
        std::cerr << "Error Code: " << errorCode << " - " << strerror(errorCode) << std::endl;
#endif
        return;
    }

    // If the file opened successfully, write to the file
    logFile << entry << std::endl;

    if (logFile.fail()) {
        int errorCode = errno;
        std::cerr << "Error writing to file: " << logFilePath << std::endl;

#if defined(_WIN32) || defined(_WIN64)
        // Windows specific error reporting
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, 
            dw, 
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
            (LPTSTR)&lpMsgBuf, 
            0, 
            NULL);

        std::cerr << "Error Code: " << dw << " - " << (char*)lpMsgBuf << std::endl;
        LocalFree(lpMsgBuf);
#else
        // POSIX (Linux) specific error reporting
        std::cerr << "Error Code: " << errorCode << " - " << strerror(errorCode) << std::endl;
#endif
    } else {
        std::cout << "Log entry written successfully." << std::endl;
    }

    // Close the log file
    logFile.close();
}

int main() {
    std::string username = "doctor"; // Suppose we get this from user input
    std::string directory = "./restricted_logs";  // This could be a restricted area
    std::string entry = "Sample audit log entry";
    
    UserType userType = getUserType(username);
    logAuditEntry(directory, entry, userType, true); // Assuming the directory is a restricted log directory

    return 0;
}