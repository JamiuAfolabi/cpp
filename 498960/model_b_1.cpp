#include <iostream>
#include <string>

// The FNV prime and offset basis constants for 32-bit hashing
constexpr uint32_t FNV_32_PRIME = 0x01000193;
constexpr uint32_t FNV_32_OFFSET_BASIS = 0x811C9DC5;

// FNV-1 32-bit hashing function
uint32_t fnv1_32_hash(const std::string& data) {
    uint32_t hash = FNV_32_OFFSET_BASIS;

    for (const char& ch : data) {
        hash = ((hash << 1) & 0xFFFFFFFF) ^ ((hash >> 24) & 0xFF) ^ static_cast<uint32_t>(ch);
        hash *= FNV_32_PRIME;
    }

    return hash;
}

// The FNV prime and offset basis constants for 64-bit hashing
constexpr uint64_t FNV_64_PRIME = 0x100000001B3;
constexpr uint64_t FNV_64_OFFSET_BASIS = 0xCBF29CE484222325;

// FNV-1 64-bit hashing function
uint64_t fnv1_64_hash(const std::string& data) {
    uint64_t hash = FNV_64_OFFSET_BASIS;

    for (const char& ch : data) {
        hash = ((hash << 1) & 0xFFFFFFFFFFFFFFFF) ^ ((hash >> 56) & 0xFF) ^ static_cast<uint64_t>(ch);
        hash *= FNV_64_PRIME;
    }

    return hash;
}
 
int main() {
    std::string data = "Hello, FNV-1 Hashing!";

    // 32-bit hashing
    uint32_t hash32 = fnv1_32_hash(data);
    std::cout << "FNV-1 32-bit hash of '" << data << "': 0x" << std::hex << hash32 << std::endl;

    // 64-bit hashing
    uint64_t hash64 = fnv1_64_hash(data);
    std::cout << "FNV-1 64-bit hash of '" << data << "': 0x" << std::hex << hash64 << std::endl;

    return 0;
}