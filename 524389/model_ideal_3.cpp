#include <iostream>
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
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Thread-safe buffer management with chunk-based storage
class ThreadSafeBufferManager {
private:
    static const size_t CHUNK_SIZE = 1024 * 1024; // 1MB chunks
    std::deque<std::vector<char>> chunks;
    size_t currentPosition;
    size_t totalSize;
    mutable std::shared_mutex mutex;  // For thread safety

public:
    ThreadSafeBufferManager() : currentPosition(0), totalSize(0) {}

    // Thread-safe append operation
    void append(const char* data, size_t length) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        while (length > 0) {
            if (chunks.empty() || chunks.back().size() == CHUNK_SIZE) {
                chunks.emplace_back();
                chunks.back().reserve(CHUNK_SIZE);
            }
            
            auto& currentChunk = chunks.back();
            size_t remaining = CHUNK_SIZE - currentChunk.size();
            size_t toWrite = std::min(length, remaining);
            
            currentChunk.insert(currentChunk.end(), data, data + toWrite);
            data += toWrite;
            length -= toWrite;
            totalSize += toWrite;
        }
    }

    // Clear buffer and reset positions
    void clear() {
        std::unique_lock<std::shared_mutex> lock(mutex);
        chunks.clear();
        currentPosition = 0;
        totalSize = 0;
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return totalSize;
    }

    // Get current character without advancing position
    char peek() const {
        std::shared_lock<std::shared_mutex> lock(mutex);
        if (currentPosition >= totalSize) return '\0';
        
        size_t chunkIndex = currentPosition / CHUNK_SIZE;
        size_t offset = currentPosition % CHUNK_SIZE;
        return chunks[chunkIndex][offset];
    }

    // Get current character and advance position
    char get() {
        std::unique_lock<std::shared_mutex> lock(mutex);
        char c = '\0';
        if (currentPosition < totalSize) {
            size_t chunkIndex = currentPosition / CHUNK_SIZE;
            size_t offset = currentPosition % CHUNK_SIZE;
            c = chunks[chunkIndex][offset];
            currentPosition++;
        }
        return c;
    }

    void seek(size_t position) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        if (position > totalSize) {
            throw std::out_of_range("Invalid buffer position");
        }
        currentPosition = position;
    }
};

// UTF-8 validation and sanitization
class UTF8Validator {
public:
    // Validate UTF-8 string
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

    // Clean invalid UTF-8 sequences
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

// Main handler for concurrent CSV processing with memory-mapped file
class ConcurrentStreamHandler {
private:
    int fileDescriptor;
    ThreadSafeBufferManager buffer;
    std::stack<std::streampos> positions;
    std::streampos lastPosition;
    bool inTransaction;
    mutable std::shared_mutex fileMutex;      // For file operations
    std::mutex transactionMutex;              // For transaction management
    std::condition_variable_any transactionCV; // For transaction coordination
    char* mappedMemory;
    size_t fileSize;

    // CSV parsing states
    enum class State {
        NORMAL,
        IN_QUOTE,
        AFTER_QUOTE,
        ERROR
    };

    // Transaction checkpoint data
    struct FileCheckpoint {
        size_t position;
        size_t bufferPosition;
        State parserState;
    };

    std::stack<FileCheckpoint> checkpoints;
    State currentState;

public:
    // Initialize handler with file
    ConcurrentStreamHandler(const std::string& filename) 
        : inTransaction(false), currentState(State::NORMAL) {
        std::unique_lock<std::shared_mutex> lock(fileMutex);
        fileDescriptor = open(filename.c_str(), O_RDWR);
        if (fileDescriptor == -1) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Get file size
        fileSize = lseek(fileDescriptor, 0, SEEK_END);
        lseek(fileDescriptor, 0, SEEK_SET);
        
        // Memory-map the file
        mappedMemory = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
        if (mappedMemory == MAP_FAILED) {
            close(fileDescriptor);
            throw std::runtime_error("Failed to memory-map file: " + filename);
        }

        readNextChunk();
    }

    ~ConcurrentStreamHandler() {
        std::unique_lock<std::shared_mutex> lock(fileMutex);
        if (mappedMemory) {
            munmap(mappedMemory, fileSize);
        }
        if (fileDescriptor != -1) {
            close(fileDescriptor);
        }
    }

    // Prevent copying
    ConcurrentStreamHandler(const ConcurrentStreamHandler&) = delete;
    ConcurrentStreamHandler& operator=(const ConcurrentStreamHandler&) = delete;

    // Start new transaction
    void beginTransaction() {
        std::unique_lock<std::mutex> transLock(transactionMutex);
        transactionCV.wait(transLock, [this] { return !inTransaction; });
        
        std::unique_lock<std::shared_mutex> fileLock(fileMutex);
        checkpoints.push({
            (size_t)(mappedMemory + buffer.size()),  // Using offset in mapped memory
            buffer.size(),
            currentState
        });
        inTransaction = true;
    }

    // Commit current transaction
    void commitTransaction() {
        std::unique_lock<std::mutex> lock(transactionMutex);
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        checkpoints.pop();
        inTransaction = false;
        transactionCV.notify_one();
    }

    // Rollback current transaction
    void rollbackTransaction() {
        std::unique_lock<std::mutex> transLock(transactionMutex);
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        {
            std::unique_lock<std::shared_mutex> fileLock(fileMutex);
            const auto& checkpoint = checkpoints.top();
            mappedMemory = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, checkpoint.position);
            buffer.seek(checkpoint.bufferPosition);
            currentState = checkpoint.parserState;
            checkpoints.pop();
        }
        
        inTransaction = false;
        transactionCV.notify_one();
    }

    // Read next CSV record
    bool readRecord(std::vector<std::string>& fields) {
        std::shared_lock<std::shared_mutex> lock(fileMutex);
        fields.clear();
        std::string field;
        bool recordComplete = false;
        
        while (!recordComplete) {
            switch (currentState) {
                case State::NORMAL: {
                    char c = buffer.peek();
                    if (c == '\0') {
                        if (!readNextChunk()) {
                            return !fields.empty();
                        }
                        continue;
                    }
                    
                    if (c == '"') {
                        currentState = State::IN_QUOTE;
                        buffer.get();
                    } else if (c == ',') {
                        fields.push_back(field);
                        field.clear();
                        buffer.get();
                    } else if (c == '\n' || c == '\r') {
                        fields.push_back(field);
                        field.clear();
                        buffer.get();
                        if (c == '\r' && buffer.peek() == '\n') {
                            buffer.get();
                        }
                        recordComplete = true;
                    } else {
                        field += buffer.get();
                    }
                    break;
                }
                
                case State::IN_QUOTE: {
                    char c = buffer.peek();
                    if (c == '\0') {
                        if (!readNextChunk()) {
                            currentState = State::ERROR;
                            throw std::runtime_error("Unexpected EOF in quoted field");
                        }
                        continue;
                    }
                    
                    if (c == '"') {
                        buffer.get();
                        if (buffer.peek() == '"') {
                            field += '"';
                            buffer.get();
                        } else {
                            currentState = State::AFTER_QUOTE;
                        }
                    } else {
                        field += buffer.get();
                    }
                    break;
                }
                
                case State::AFTER_QUOTE: {
                    char c = buffer.peek();
                    if (c == ',') {
                        fields.push_back(field);
                        field.clear();
                        buffer.get();
                        currentState = State::NORMAL;
                    } else if (c == '\n' || c == '\r') {
                        fields.push_back(field);
                        field.clear();
                        buffer.get();
                        if (c == '\r' && buffer.peek() == '\n') {
                            buffer.get();
                        }
                        currentState = State::NORMAL;
                        recordComplete = true;
                    } else {
                        currentState = State::ERROR;
                        throw std::runtime_error("Invalid character after quote");
                    }
                    break;
                }
                
                case State::ERROR:
                    throw std::runtime_error("Parser in error state");
            }
        }
        
        // Validate all fields
        for (auto& f : fields) {
            if (!UTF8Validator::isValid(f)) {
                f = UTF8Validator::sanitize(f);
            }
        }
        
        return true;
    }

private:
    // Read next chunk of data
    bool readNextChunk() {
        if (buffer.size() >= fileSize) {
            return false;
        }

        size_t chunkSize = std::min((size_t)1024, fileSize - buffer.size());
        buffer.append(mappedMemory + buffer.size(), chunkSize);

        return true;
    }
};

// Example usage with multiple threads
int main() {
    try {
        ConcurrentStreamHandler handler("transactions.csv");
        const int numThreads = 4;
        std::vector<std::thread> threads;

        // Create reader threads
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

        // Wait for completion
        for (auto& thread : threads) {
            thread.join();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return 0;
}
