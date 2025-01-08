#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <CommonCrypto/CommonDigest.h>
#include <sstream>
#include <iomanip>
#include <thread>  // Include the thread library
#include <mutex>   // Include the mutex library for thread safety

using namespace std;

// Function to initialize the s-box using a provided key
vector<int> initializeSBox(vector<int> s, const vector<char>& key);

// Function to obfuscate the BVN (Bank Verification Number)
string obfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);

// Function to deobfuscate the BVN
string deobfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);

// Function to generate a random key of specified length
vector<char> generateRandomKey(size_t length);

// Function to hash a string using SHA-256 (CommonCrypto)
string hashSHA256(const string& input);

// Function to compare two hashes
bool compareHashes(const string& originalHash, const string& newHash);

// Function to obfuscate multiple BVNs in parallel
void obfuscateBVNsInParallel(vector<int> s, const vector<char>& key, const vector<string>& bvnList, vector<string>& obfuscatedResults, vector<string>& deobfuscatedResults, vector<bool>& hashResults);

int main() {
    vector<string> bvnList = {
        "1234567890123456", // Example BVN 1
        "2345678901234567", // Example BVN 2
        "3456789012345678"  // Example BVN 3
    };

    vector<int> S(256); // s-box initialization vector
    vector<char> key = generateRandomKey(16); // Generate random key of 16 bytes (128 bits)
    
    // Initialize the s-box with the key
    S = initializeSBox(S, key);

    // Vectors to store obfuscated BVNs, deobfuscated BVNs, and hash comparison results
    vector<string> obfuscatedResults(bvnList.size());
    vector<string> deobfuscatedResults(bvnList.size());
    vector<bool> hashResults(bvnList.size());

    // Obfuscate BVNs in parallel and check hash matches
    obfuscateBVNsInParallel(S, key, bvnList, obfuscatedResults, deobfuscatedResults, hashResults);

    // Print the results
    for (size_t i = 0; i < bvnList.size(); i++) {
        cout << "Original BVN: " << bvnList[i] << endl;
        cout << "Obfuscated BVN: " << obfuscatedResults[i] << endl;
        cout << "Deobfuscated BVN: " << deobfuscatedResults[i] << endl;
        cout << "Hashes match: " << (hashResults[i] ? "Yes" : "No") << endl;
        cout << endl;  // Ensure a space between each result
    }

    return 0;
}

vector<char> generateRandomKey(size_t length) {
    vector<char> key(length);
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, 255);

    for (size_t i = 0; i < length; ++i) {
        key[i] = static_cast<char>(distribution(generator));
    }
    return key;
}

string obfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int c;
    string cipher_T;

    for (size_t r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i], s[j]);

        int tmp_index = (s[i] + s[j]) % 256;
        k = s[tmp_index];

        c = (static_cast<int>(bvn[r]) ^ k);
        cipher_T += static_cast<char>(c);
    }
    return cipher_T;
}

string deobfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int c;
    string plain_T;

    for (size_t r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i], s[j]);

        int tmp_index = (s[i] + s[j]) % 256;
        k = s[tmp_index];

        c = (static_cast<int>(bvn[r]) ^ k);
        plain_T += static_cast<char>(c);
    }
    return plain_T;
}

vector<int> initializeSBox(vector<int> s, const vector<char>& key) {
    int b = 0;
    for (int a = 0; a < 256; a++) {
        s[a] = a;
    }
    for (int a = 0; a < 256; a++) {
        b = (b + s[a] + key[a % key.size()]) % 256;
        swap(s[a], s[b]);
    }
    return s;
}

string hashSHA256(const string& input) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(input.c_str(), static_cast<CC_LONG>(input.length()), hash);

    stringstream hexHash;
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) {
        hexHash << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return hexHash.str();
}

bool compareHashes(const string& originalHash, const string& newHash) {
    return originalHash == newHash;
}

// Function to obfuscate multiple BVNs concurrently and check hash matches
void obfuscateBVNsInParallel(vector<int> s, const vector<char>& key, const vector<string>& bvnList, vector<string>& obfuscatedResults, vector<string>& deobfuscatedResults, vector<bool>& hashResults) {
    vector<thread> threads;
    mutex mtx;

    for (size_t i = 0; i < bvnList.size(); i++) {
        threads.push_back(thread([&, i]() {
            // Obfuscate the BVN
            string obfuscated = obfuscateBVN(s, key, bvnList[i]);
            string deobfuscated = deobfuscateBVN(s, key, obfuscated);

            // Hash the original and deobfuscated BVNs
            string originalHash = hashSHA256(bvnList[i]);
            string deobfuscatedHash = hashSHA256(deobfuscated);

            // Compare the hashes
            bool isVerified = compareHashes(originalHash, deobfuscatedHash);

            // Store the results in the shared vectors safely
            {
                lock_guard<mutex> lock(mtx);
                obfuscatedResults[i] = obfuscated;
                deobfuscatedResults[i] = deobfuscated;
                hashResults[i] = isVerified;
            }
        }));
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
}
