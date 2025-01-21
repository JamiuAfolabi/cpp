#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <system_error>
#ifdef _WIN32
#include <Windows.h>
#endif

#define AUDIT_LOG_FILE_PATH "./"  // Replace this with your desired directory path and file name

void createAndWriteAuditLog(const std::string& logMessage)
{
    std::error_code ec;

    try {
        std::ofstream auditLog(AUDIT_LOG_FILE_PATH, std::ios::app);
        if (!auditLog) {
            throw std::ios_base::failure("Failed to open audit log file for writing.");
        }

        auditLog << logMessage << std::endl;
        auditLog.close();
    } catch (const std::exception& e) {
#ifdef _WIN32
        DWORD lastError = GetLastError();
        std::wcerr << L"Error: " << e.what() << L" (Windows Error Code: " << lastError << L")\n";
        std::wcerr << L"Error Description: " << std::get<std::wstring>(std::system_category().messages(lastError)) << L"\n";
#else
        std::cerr << "Error: " << e.what() << " (Linux Error Code: " << errno << ")\n";
        std::cerr << "Error Description: " << std::strerror(errno) << "\n";
#endif
    }
}

int main() {
    std::string auditLogMessage = "This is an audit log message.";
    createAndWriteAuditLog(auditLogMessage);
    return 0;
}
 