#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <Foundation/Foundation.h>

using namespace std;

vector<int> initializeSBox(vector<int> s, const vector<char>& key);
string obfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);
string deobfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);
vector<char> generateRandomKey(size_t length);

// Function to calculate SHA-256 hash of a string using CommonCrypto
string calculateSHA256(const string& data) {
    const char *cstr = data.c_str();
    uint8_t digest[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(cstr, static_cast<uint32_t>(data.length()), digest);

    char hash[CC_SHA256_DIGEST_LENGTH * 2];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + i * 2, "%02hhx", digest[i]);
    }

    return string(hash);
}

bool verifySHA256(const string& data, const string& expectedHash) {
    string actualHash = calculateSHA256(data);
    return actualHash == expectedHash;
}

int main() {
    string bvn = "1234567890123456";  // Example BVN
    vector<int> S(256); // s-box initialization vector

    vector<char> key = generateRandomKey(16);
    S = initializeSBox(S, key);

    string obfuscatedBVN = obfuscateBVN(S, key, bvn);

    // Calculate SHA-256 hash of the original BVN
    string bvnHash = calculateSHA256(bvn);
    cout << "SHA-256 Hash of Original BVN: " << bvnHash << endl;

    // Store obfuscated data and its hash for transmission or storage
    struct {
        string obfuscatedData;
        string hash;
    } encryptedData;
    encryptedData.obfuscatedData = obfuscatedBVN;
    encryptedData.hash = bvnHash;

    // Let's assume we transmit the 'encryptedData' struct
    // ...

    // Upon receiving the obfuscated data and hash
    string receivedObfuscatedBVN = encryptedData.obfuscatedData;
    string receivedHash = encryptedData.hash;

    string deobfuscatedBVN = deobfuscateBVN(S, key, receivedObfuscatedBVN);

    // Verify the integrity of the data
    if (!verifySHA256(deobfuscatedBVN, receivedHash)) {
        cout << "Data integrity check failed! The data has been tampered with." << endl;
    } else {
        cout << "Data integrity check passed." << endl;
    }

    cout << "Original BVN: " << bvn << endl;
    cout << "Obfuscated BVN: " << receivedObfuscatedBVN << endl;
    cout << "Deobfuscated BVN: " << deobfuscatedBVN << endl;

    return 0;
}
