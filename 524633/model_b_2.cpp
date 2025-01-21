#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string.h>
#include <sddl.h>  // For Security Descriptor Definitions
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <execinfo.h>
#include <cstring> // for strerror
#include <sys/acl.h>  // For Access Control Lists
#include <grp.h>     // For getting group gid
#endif

// User permission levels
enum UserPermission {
    USER_ADMIN,
    USER_DOCTOR,
    USER_NURSE
};

const char* userPermissionToString[] = {
    "ADMIN",
    "DOCTOR",
    "NURSE"
};

bool checkPermission(UserPermission requiredPermission, UserPermission currentPermission) {
    switch (requiredPermission) {
        case USER_ADMIN:
            return currentPermission == USER_ADMIN;
        case USER_DOCTOR:
            return currentPermission == USER_ADMIN || currentPermission == USER_DOCTOR;
        case USER_NURSE:
            return currentPermission == USER_ADMIN || currentPermission == USER_DOCTOR || currentPermission == USER_NURSE;
    }
    return false;
}

void logAuditEntry(const std::string& directory, const std::string& entry, UserPermission permissionLevel) {
    // Create a log file path in the specified directory
    std::string logFilePath = directory + "/audit_log.txt";
    
    std::ofstream logFile;
    UserPermission currentPermission = USER_NURSE; // Replace this with the actual current user permission level
    bool allowAccess = checkPermission(permissionLevel, currentPermission);

    if (allowAccess) {
#if defined(_WIN32) || defined(_WIN64)
        SECURITY_DESCRIPTOR sd;
        DWORD dwRet = InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        if (dwRet != ERROR_SUCCESS) {
            std::cerr << "Failed to initialize security descriptor: " << dwRet << std::endl;
            return;
        }

        PSECURITY_ATTRIBUTES pSA = nullptr;
        if (!CreateSecurityDescriptorA(
                &sd,                   // dwRevision - [in]   
                NULL,                   // pOwningDomain - [in]
                SE_FILE_OBJECT_TYPE,   // Type - [in]
                GENERIC_WRITE | GENERIC_READ,   // GenericMask - [in]
                NULL,                   // pDacl - [in]
                NULL,                   // pSacl - [in]
                DACL_PROTECTED,        // bDaclInherit - [in]
                SACL_PROTECTED         // bSaclInherit - [in]
            )) {
            std::cerr << "Error creating security descriptor: " << GetLastError() << std::endl;
            return;
        }

        // Define ACEs for different permission levels (ADMIN, DOCTOR, NURSE)
        EXPLICIT_ACCESS ea[3] = {
            { // ACE for ADMINS
                ACL_REVISION,
                NO_INHERIT_CONDITION,
                ACCESS_ALLOWED_ACE_TYPE,
                GENERIC_READ | GENERIC_WRITE,
                (SID*)LookupWellKnownSid(WinBuiltinAdministratorsSid, NULL)
            },
            { // ACE for DOCTORS (inherited from ADMINS)
                ACL_REVISION,
                CONDITIONAL_INHERIT_ACE_FLAG | NO_PROPAGATE_INHERIT_ACE_FLAG | INHERIT_ONLY_ACE_FLAG,
                ACCESS_ALLOWED_ACE_TYPE,
                GENERIC_READ,
                (SID*)LookupWellKnownSid(WinBuiltinAdministratorsSid, NULL)
            },
            { // ACE for NURSES (inherited from DOCTORS)
                ACL_REVISION,
                CONDITIONAL_INHERIT_ACE_FLAG | NO_PROPAGATE_INHERIT_ACE_FLAG | INHERIT_ONLY_ACE_FLAG,
                ACCESS_ALLOWED_ACE_TYPE,
                FILE_READ_ATTRIBUTES | SYNCHRONIZE,
                (SID*)LookupWellKnownSid(WinBuiltinUsersSid, NULL)
            }
        };

        // Create the DACL
        PACL pDacl;
        if (!CreateAccessList(
                ea, 
                ARRAY_SIZE(ea),
                sizeof(EXPLICIT_ACCESS),
                ACL_REVISION,
                &pDacl
            )) {
            std::cerr << "Error creating access control list: " << GetLastError() << std::endl;
            return;
        }

        // Set the DACL on the security descriptor
        SetSecurityDescriptorDacl(&sd, TRUE, pDacl, FALSE);

        // Remove the default security descriptor
        LocalFree(pSA);

        // Set the security descriptor on the file handle
        pSA = (PSECURITY_ATTRIBUTES)LocalAlloc(LPTR, sizeof(SECURITY_ATTRIBUTES));
        if (pSA == NULL) {
            std::cerr << "Error allocating memory for security attributes: " << GetLastError() << std::endl;
            return;
        }

        pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
        pSA->lpSecurityDescriptor = &sd;
        pSA->bInheritHandle = FALSE;

        // Open the file handle with security attributes
        HANDLE hFile = CreateFileA(
            logFilePath.c_str(),
            GENERIC_WRITE | GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            pSA,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | SECURITY_ATTRIBUTE_INHERIT,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            int errorCode = GetLastError();
            std::cerr << "Error opening file with security attributes: " << logFilePath << std::endl;
            std::cerr << "Error Code: " << errorCode << " - " << GetLastError() << std::endl;
            LocalFree(pSA);
            return;
        }

        DWORD dwBytesWritten;
        if (!WriteFile(hFile, entry.c_str(), entry.length(), &dwBytesWritten, NULL)) {
            int errorCode = GetLastError();
            std::cerr << "Error writing to file: " << logFilePath << std::endl;
            std::cerr << "Error Code: " << errorCode << " - " << GetLastError() << std::endl;
        } else {
            std::cout << "Log entry written successfully." << std::endl;
        }

        CloseHandle(hFile);
        LocalFree(pSA);

#else
        // Posix (Linux) equivalent code
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; // 0660 permissions
        struct acl *acl;
        struct acl_entry *ace;
        struct passwd *pwd;
        struct group *grp;

        pwd = getpwnam("admin"); // Assuming admin UID is 0 on Linux
        if (pwd == NULL) {
            std::cerr << "Error looking up admin user: " << strerror(errno) << std::endl;
            return;
        }

        grp = getgrnam("admin"); // Assuming admin GID is 0 on Linux
        if (grp == NULL) {
            std::cerr << "Error looking up admin group: " << strerror(errno) << std::endl;
            return;
        }

        acl = acl_new();
        if (acl == NULL) {
            std::cerr << "Error creating ACL: " << strerror(errno) << std::endl;
            return;
        }

        ace = acl_entry_new(ACL_TYPE_USER, pwd->pw_uid);
        if (ace == NULL) {
            std::cerr << "Error creating user ACL entry: " << strerror(errno) << std::endl;
            acl_free(acl);
            return;
        }

        if (acl_set_permset(ace, A_UR | A_UW)) {
            std::cerr << "Error setting user permissions: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        if (acl_set_entry(acl, ace) == -1) {
            std::cerr << "Error adding user entry to ACL: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        ace = acl_entry_new(ACL_TYPE_GROUP, grp->gr_gid);
        if (ace == NULL) {
            std::cerr << "Error creating group ACL entry: " << strerror(errno) << std::endl;
            acl_free(acl);
            return;
        }

        if (acl_set_permset(ace, A_GR | A_GW)) {
            std::cerr << "Error setting group permissions: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        if (acl_set_entry(acl, ace) == -1) {
            std::cerr << "Error adding group entry to ACL: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        ace = acl_entry_new(ACL_TYPE_MASK);
        if (ace == NULL) {
            std::cerr << "Error creating mask ACL entry: " << strerror(errno) << std::endl;
            acl_free(acl);
            return;
        }

        if (acl_set_permset(ace, A_UR | A_UW | A_GR | A_GW)) {
            std::cerr << "Error setting mask permissions: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        if (acl_set_entry(acl, ace) == -1) {
            std::cerr << "Error adding mask entry to ACL: " << strerror(errno) << std::endl;
            acl_entry_free(ace);
            acl_free(acl);
            return;
        }

        // At this point, we have ACL set up with admin user and group having full access

        int fd = open(logFilePath.c_str(), O_WRONLY | O_CREAT | O_APPEND, mode);
        if (fd == -1) {
            int errorCode = errno;
            std::cerr << "Error opening file with ACL: " << logFilePath << std::endl;
            std::cerr << "Error Code: " << errorCode << " - " << strerror(errorCode) << std::endl;
            acl_free(acl);
            return;
        }

        if (setfacl(logFilePath.c_str(), acl) == -1) {
            std::cerr << "Error setting ACL on file: " << logFilePath << std::endl;
            std::cerr << "Error Code: " << errno << " - " << strerror(errno) << std::endl;
        }

        if (chmod(logFilePath.c_str(), mode) == -1) {
            std::cerr << "Error changing file mode: " << logFilePath << std::endl;
            std::cerr << "Error Code: " << errno << " - " << strerror(errno) << std::endl;
        }

        logFile.fd(fd);
#endif

        logFile << entry << std::endl;
    } else {
        std::cerr << "Access Denied! User " << userPermissionToString[currentPermission] << " does not have permission to access this directory." << std::endl;
    }

    // Close the log file
    logFile.close();
}

int main() {
    std::string directory = "./restricted_logs"; // Replace this with the actual restricted directory path
    std::string entry = "Sample audit log entry by a user with admin permission";
    logAuditEntry(directory, entry, USER_ADMIN);

    return 0;
}