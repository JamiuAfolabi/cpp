#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <cstdlib>

// Check for Windows or Linux
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <cstring> // for strerror
#endif

void logAuditEntry(const std::string& directory, const std::string& entry) {
    // Create a log file path in the specified directory
    std::string logFilePath = directory + "/audit_log.txt";

    std::ofstream logFile;
    logFile.open(logFilePath, std::ios::app);

    // Check if the file opened properly
    if (!logFile) {
        int errorCode = errno; // Get the error code
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
            (LPTSTR) &lpMsgBuf, 
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
        int errorCode = errno; // Get the error code
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
            (LPTSTR) &lpMsgBuf, 
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
    std::string directory = "/asdf";
    std::string entry = "Sample audit log entry";

    logAuditEntry(directory, entry);
    return 0;
}