#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <condition_variable>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

class MemoryMappedFile {
private:
    int fd;
    void* mapAddr;
    size_t size;

public:
    MemoryMappedFile(const std::string& filename) {
        fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Failed to open file");
        }

        struct stat fileInfo;
        if (fstat(fd, &fileInfo) != 0) {
            close(fd);
            throw std::runtime_error("Failed to get file information");
        }

        size = fileInfo.st_size;
        mapAddr = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mapAddr == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("Failed to map file to memory");
        }
    }

    ~MemoryMappedFile() {
        munmap(mapAddr, size);
        close(fd);
    }

    const char* getData() const {
        return static_cast<const char*>(mapAddr);
    }

    size_t getSize() const {
        return size;
    }
};

class MemoryMappedBuffer {
private:
    const char* data;
    size_t position;
    size_t totalSize;
    mutable std::shared_mutex mutex;

public:
    MemoryMappedBuffer(const MemoryMappedFile& file)
        : data(file.getData()), position(0), totalSize(file.getSize()) {}

    const char* peek() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        if (position >= totalSize) return nullptr;
        return data + position;
    }

    const char* get() {
        std::unique_lock<std::shared_mutex> lock(mutex);
        const char* ret = peek();
        if (ret != nullptr) position++;
        return ret;
    }

    void seek(size_t pos) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        if (pos > totalSize) {
            throw std::out_of_range("Invalid buffer position");
        }
        position = pos;
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return totalSize;
    }

    size_t tell() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return position;
    }
};

class UTF8Validator {
public:
    static bool isValid(const std::string& str) {
        const unsigned char* bytes = (const unsigned char*)str.c_str();
        size_t len = str.length();
        
        for (size_t i = 0; i < len; i++) {
            if (bytes[i] <= 0x7F) continue;
            
            if (bytes[i] >= 0xC2 && bytes[i] <= 0xDF) {
                if (i + 1 >= len || (bytes[i + 1] & 0xC0) != 0x80)
                    return false;
                i += 1;
            } else if (bytes[i] == 0xE0) {
                if (i + 2 >= len || (bytes[i + 1] & 0xE0) != 0xA0 || 
                    (bytes[i + 2] & 0xC0) != 0x80)
                    return false;
                i += 2;
            } else if (bytes[i] >= 0xE1 && bytes[i] <= 0xEF) {
                if (i + 2 >= len || (bytes[i + 1] & 0xC0) != 0x80 || 
                    (bytes[i + 2] & 0xC0) != 0x80)
                    return false;
                i += 2;
            } else {
                return false;
            }
        }
        return true;
    }

    static std::string sanitize(const std::string& str) {
        std::string result;
        const unsigned char* bytes = (const unsigned char*)str.c_str();
        size_t len = str.length();
        
        for (size_t i = 0; i < len; i++) {
            if (bytes[i] <= 0x7F) {
                result += bytes[i];
            } else if (bytes[i] >= 0xC2 && bytes[i] <= 0xDF && i + 1 < len) {
                result += bytes[i];
                result += bytes[++i];
            } else if (bytes[i] == 0xE0 && i + 2 < len) {
                result += bytes[i];
                result += bytes[++i];
                result += bytes[++i];
            } else if (bytes[i] >= 0xE1 && bytes[i] <= 0xEF && i + 2 < len) {
                result += bytes[i];
                result += bytes[++i];
                result += bytes[++i];
            } else {
                result += '?'; // Replace invalid sequences
            }
        }
        return result;
    }
};

class ConcurrentStreamHandler {
private:
    MemoryMappedFile file;
    MemoryMappedBuffer buffer;
    std::stack<std::streampos> positions;
    std::streampos lastPosition;
    bool inTransaction;
    mutable std::shared_mutex fileMutex;
    std::mutex transactionMutex;
    std::condition_variable_any transactionCV;

    enum class State {
        NORMAL,
        IN_QUOTE,
        AFTER_QUOTE,
        ERROR
    };

    struct FileCheckpoint {
        std::streampos position;
        State parserState;
    };

    std::stack<FileCheckpoint> checkpoints;
    State currentState;

public:
    ConcurrentStreamHandler(const std::string& filename) 
        : file(filename), buffer(file), inTransaction(false), currentState(State::NORMAL) {}

    ~ConcurrentStreamHandler() {}

    void beginTransaction() {
        std::unique_lock<std::mutex> transLock(transactionMutex);
        transactionCV.wait(transLock, [this] { return !inTransaction; });
        
        std::unique_lock<std::shared_mutex> fileLock(fileMutex);
        checkpoints.push({
            buffer.tell(),
            currentState
        });
        inTransaction = true;
    }

    void commitTransaction() {
        std::unique_lock<std::mutex> lock(transactionMutex);
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        checkpoints.pop();
        inTransaction = false;
        transactionCV.notify_one();
    }

    void rollbackTransaction() {
        std::unique_lock<std::mutex> transLock(transactionMutex);
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        {
            std::unique_lock<std::shared_mutex> fileLock(fileMutex);
            const auto& checkpoint = checkpoints.top();
            buffer.seek(checkpoint.position);
            currentState = checkpoint.parserState;
            checkpoints.pop();
        }
        
        inTransaction = false;
        transactionCV.notify_one();
    }

    bool readRecord(std::vector<std::string>& fields) {
        std::shared_lock<std::shared_mutex> lock(fileMutex);
        fields.clear();
        std::string field;
        bool recordComplete = false;
        
        while (!recordComplete) {
            switch (currentState) {
                case State::NORMAL: {
                    char c = buffer.get();
                    if (c == '\0') {
                        return !fields.empty();
                    }
                    
                    if (c == '"') {
                        currentState = State::IN_QUOTE;
                    } else if (c == ',') {
                        fields.push_back(field);
                        field.clear();
                    } else if (c == '\n' || c == '\r') {
                        fields.push_back(field);
                        field.clear();
                        if (c == '\r' && buffer.peek() == '\n') {
                            buffer.get();
                        }
                        recordComplete = true;
                    } else {
                        field += c;
                    }
                    break;
                }
                
                case State::IN_QUOTE: {
                    char c = buffer.get();
                    if (c == '\0') {
                        throw std::runtime_error("Unexpected EOF in quoted field");
                    }
                    
                    if (c == '"') {
                        if (buffer.get() == '"') {
                            field += '"';
                        } else {
                            currentState = State::AFTER_QUOTE;
                        }
                    } else {
                        field += c;
                    }
                    break;
                }
                
                case State::AFTER_QUOTE: {
                    char c = buffer.get();
                    if (c == ',') {
                        fields.push_back(field);
                        field.clear();
                        currentState = State::NORMAL;
                    } else if (c == '\n' || c == '\r') {
                        fields.push_back(field);
                        field.clear();
                        if (c == '\r' && buffer.peek() == '\n') {
                            buffer.get();
                        }
                        currentState = State::NORMAL;
                        recordComplete = true;
                    } else {
                        throw std::runtime_error("Invalid character after quote");
                    }
                    break;
                }
                
                case State::ERROR:
                    throw std::runtime_error("Parser in error state");
            }
        }
        
        for (auto& f : fields) {
            if (!UTF8Validator::isValid(f)) {
                f = UTF8Validator::sanitize(f);
            }
        }
        
        return true;
    }

    void seek(std::streampos position) {
        std::unique_lock<std::shared_mutex> lock(fileMutex);
        if (position < 0) {
            throw std::out_of_range("Invalid seek position");
        }
        
        buffer.seek(position);
    }

    std::streampos tell() const {
        std::shared_lock<std::shared_mutex> lock(fileMutex);
        return buffer.tell();
    }
};

int main() {
    try {
        ConcurrentStreamHandler handler("transactions.csv");
        const int numThreads = 4;
        std::vector<std::thread> threads;
        
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&handler, i]() {
                try {
                    std::vector<std::string> record;
                    handler.beginTransaction();
                    
                    while (handler.readRecord(record)) {
                        std::cout << "Thread " << i << ": ";
                        for (const auto& field : record) {
                            std::cout << field << "\t";
                        }
                        std::cout << "\n";
                    }
                    
                    handler.commitTransaction();
                } catch (const std::exception& e) {
                    std::cerr << "Thread " << i << " error: " << e.what() << "\n";
                    handler.rollbackTransaction();
                }
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return 0;
}
