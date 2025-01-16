#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

class MemoryMappedFile {
private:
    int fd;
    char *mappedData;
    size_t fileSize;

public:
    MemoryMappedFile(const std::string& filename) : fd(-1), mappedData(MAP_FAILED), fileSize(0) {
        // Open the file
        fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Get file size
        fileSize = lseek(fd, 0, SEEK_END);
        if (fileSize == -1) {
            close(fd);
            throw std::runtime_error("Failed to determine file size.");
        }

        // Memory-map the file
        mappedData = (char *)mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);
        if (mappedData == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("Failed to map file to memory.");
        }
    }

    ~MemoryMappedFile() {
        if (mappedData != MAP_FAILED) {
            munmap(mappedData, fileSize);
        }
        if (fd != -1) {
            close(fd);
        }
    }

    const char* data() const {
        return mappedData;
    }

    size_t size() const {
        return fileSize;
    }
};

class CSVParser {
public:
    static std::vector<std::string> parseRecord(const char *data, size_t length) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;

        for (size_t i = 0; i < length; ++i) {
            char c = data[i];

            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < length && data[i + 1] == '"') {
                        // Escaped quote
                        field += '"';
                        ++i; // Skip next quote
                    } else {
                        inQuotes = false; // Closing quote
                    }
                } else {
                    field += c;
                }
            } else {
                if (c == '"') {
                    inQuotes = true; // Opening quote
                } else if (c == ',') {
                    fields.push_back(field);
                    field.clear();
                } else if (c == '\n') {
                    fields.push_back(field);
                    field.clear();
                    return fields; // End of record
                } else {
                    field += c;
                }
            }
        }
        // Add last field if not empty
        if (!field.empty()) {
            fields.push_back(field);
        }
        return fields;
    }
};

// Thread-safe function to parse CSV concurrently cusing memory-mapped file
void parseCSV(const char* data, size_t size) {
    size_t recordStart = 0;

    while (recordStart < size) {
        // Find the end of the current record
        size_t recordEnd = recordStart;
        while (recordEnd < size && data[recordEnd] != '\n') {
            ++recordEnd;
        }

        // Parse the record
        auto fields = CSVParser::parseRecord(data + recordStart, recordEnd - recordStart);
        
        // Output the parsed fields
        std::cout << "Record: ";
        for (const auto& field : fields) {
            std::cout << field << "\t";
        }
        std::cout << "\n";

        // Move to the start of the next record
        recordStart = recordEnd + 1; // Move past the newline
    }
}

// Example usage
int main(int argc, char* argv[]) {
    const std::string filename = "transactions.csv";

    try {
        MemoryMappedFile mmf(filename);
        const char* data = mmf.data();
        size_t size = mmf.size();

        // You may create threads to process concurrently.
        std::thread t1(parseCSV, data, size);
        std::thread t2(parseCSV, data, size);

        t1.join();
        t2.join();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}