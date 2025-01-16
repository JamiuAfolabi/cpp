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
#include <condition_variable>


class BufferManager {
private:
    static const size_t CHUNK_SIZE = 1024 * 1024; // 1MB chunks
    std::deque<std::vector<char>> chunks;
    size_t currentPosition;
    size_t totalSize;
    std::mutex buffer_mutex; 

public:
    BufferManager() : currentPosition(0), totalSize(0) {}

    void append(const char* data, size_t length) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
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

    void clear() {
        chunks.clear();
        currentPosition = 0;
        totalSize = 0;
    }

    size_t size() const { return totalSize; }

    // Get data at current position
    char peek() const {
        if (currentPosition >= totalSize) return '\0';
        
        size_t chunkIndex = currentPosition / CHUNK_SIZE;
        size_t offset = currentPosition % CHUNK_SIZE;
        return chunks[chunkIndex][offset];
    }

    // Move position forward and return character
    char get() {
        char c = peek();
        if (c != '\0') currentPosition++;
        return c;
    }

    void seek(size_t position) {
        if (position > totalSize) {
            throw std::out_of_range("Invalid buffer position");
        }
        currentPosition = position;
    }
};

class UTF8Validator {
public:
    static bool isValid(const std::string& str) {
        const unsigned char* bytes = (const unsigned char*)str.c_str();
        size_t len = str.length();
        
        for (size_t i = 0; i < len; i++) {
            if (bytes[i] <= 0x7F) {
                continue;
            } else if (bytes[i] >= 0xC2 && bytes[i] <= 0xDF) {
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

class StreamHandler {
private:
    std::ifstream file;
    BufferManager buffer;
    std::stack<std::streampos> positions;
    std::streampos lastPosition;
    bool inTransaction;

    std::mutex file_mutex;
    std::condition_variable data_ready;
    bool stop = false;
    std::thread reader_thread;
    
    enum class State {
        NORMAL,
        IN_QUOTE,
        AFTER_QUOTE,
        ERROR
    };

    struct FileCheckpoint {
        std::streampos position;
        size_t bufferPosition;
        State parserState;
    };

    std::stack<FileCheckpoint> checkpoints;
    State currentState;

public:
    StreamHandler(const std::string& filename) 
        : inTransaction(false), currentState(State::NORMAL) {
        file.open(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        
        // Read initial chunk
        readNextChunk();
    }

    ~StreamHandler() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Prevent copying
    StreamHandler(const StreamHandler&) = delete;
    StreamHandler& operator=(const StreamHandler&) = delete;

    void beginTransaction() {
        if (inTransaction) {
            throw std::runtime_error("Transaction already in progress");
        }
        
        checkpoints.push({
            file.tellg(),
            buffer.size(),
            currentState
        });
        inTransaction = true;
    }

    void commitTransaction() {
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        checkpoints.pop();
        inTransaction = false;
    }

    void rollbackTransaction() {
        if (!inTransaction) {
            throw std::runtime_error("No transaction in progress");
        }
        
        const auto& checkpoint = checkpoints.top();
        file.seekg(checkpoint.position);
        buffer.seek(checkpoint.bufferPosition);
        currentState = checkpoint.parserState;
        
        checkpoints.pop();
        inTransaction = false;
    }

    bool readRecord(std::vector<std::string>& fields) {
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
                        buffer.get(); // consume quote
                    } else if (c == ',') {
                        fields.push_back(field);
                        field.clear();
                        buffer.get(); // consume comma
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
        
        // Validate UTF-8 encoding for all fields
        for (auto& f : fields) {
            if (!UTF8Validator::isValid(f)) {
                f = UTF8Validator::sanitize(f);
            }
        }
        
        return true;
    }

    void seek(std::streampos position) {
        if (position < 0) {
            throw std::out_of_range("Invalid seek position");
        }
        
        file.seekg(position);
        if (file.fail()) {
            throw std::runtime_error("Seek operation failed");
        }
        
        buffer.clear();
        currentState = State::NORMAL;
        readNextChunk();
    }

    std::streampos tell() {
        return file.tellg() - std::streampos(buffer.size());
    }

    // If we need a const version, we can cache the position
    std::streampos tellConst() const {
        return lastPosition - std::streampos(buffer.size());
    }

private:
    bool readNextChunk() {
        if (file.eof()) {
            return false;
        }
        
        char chunk[8192];
        file.read(chunk, sizeof(chunk));
        
        if (file.bad()) {
            throw std::runtime_error("Error reading from file");
        }
        
        size_t bytesRead = file.gcount();
        if (bytesRead > 0) {
            buffer.append(chunk, bytesRead);
            return true;
        }
        
        return false;
    }
    void load_next_chunk_into_buffer() {
        std::unique_lock<std::mutex> lock(file_mutex);
    }
    void close() {
        {
            std::lock_guard<std::mutex> lock(file_mutex);
            stop = true;
        }
        data_ready.notify_all();
        reader_thread.join();
    }
};

// Example usage
int main() {
    try {
        StreamHandler handler("transactions.csv");
        std::vector<std::string> record;
        
        // Start transaction
        handler.beginTransaction();
        
        try {
            // Read some records
            while (handler.readRecord(record)) {
                for (const auto& field : record) {
                    std::cout << field << "\t";
                }
                std::cout << "\n";
            }
            
            // Commit if everything is okay
            handler.commitTransaction();
        } catch (const std::exception& e) {
            std::cerr << "Error processing record: " << e.what() << "\n";
            // Roll back on error
            handler.rollbackTransaction();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return 0;
}