#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <CommonCrypto/CommonCrypto.h>  // Include CommonCrypto for SHA-256
using namespace std;

// Function to initialize the s-box using a provided key
vector<int> initializeSBox(vector<int> s, const vector<char>& key);

// Function to obfuscate the BVN (Bank Verification Number)
string obfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);

// Function to deobfuscate the BVN
string deobfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn);

// Function to generate a random key of specified length
vector<char> generateRandomKey(size_t length);

// Function to compute SHA-256 hash
string computeSHA256(const string& data);

int main() {
    string bvn = "1234567890123456";  // Example BVN
    vector<int> S(256); // s-box initialization vector

    // Generate a random key of 16 bytes (128 bits)
    vector<char> key = generateRandomKey(16);

    // Initialize the s-box with the key
    S = initializeSBox(S, key);

    // Obfuscate the BVN
    string obfuscatedBVN = obfuscateBVN(S, key, bvn);

    // Compute the SHA-256 hash of the original BVN for integrity check
    string originalHash = computeSHA256(bvn);

    // Deobfuscate the BVN
    string deobfuscatedBVN = deobfuscateBVN(S, key, obfuscatedBVN);
    
    // Compute the SHA-256 hash of the deobfuscated BVN for integrity check
    string deobfuscatedHash = computeSHA256(deobfuscatedBVN);

    // Verify integrity
    if (originalHash == deobfuscatedHash) {
        cout << "Integrity check passed. Data is intact." << endl;
    } else {
        cout << "Integrity check failed. Data may be tampered with!" << endl;
    }

    // Print the results
    cout << "Original BVN: " << bvn << endl;
    cout << "Obfuscated BVN: " << obfuscatedBVN << endl;
    cout << "Deobfuscated BVN: " << deobfuscatedBVN << endl;

    return 0;
}

vector<char> generateRandomKey(size_t length) {
    vector<char> key(length);
    random_device rd;  // Obtain a random number from hardware
    mt19937 generator(rd()); // Seed the generator
    uniform_int_distribution<> distribution(0, 255); // Define the range

    for (size_t i = 0; i < length; ++i) {
        key[i] = static_cast<char>(distribution(generator)); // Generate random byte
    }
    return key;
}

string computeSHA256(const string& data) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(data.c_str(), static_cast<CC_LONG>(data.size()), hash);

    string hashString;
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++) {
        hashString += sprintf("%02x", hash[i]); // Convert to hex
    }
    return hashString;
}

string deobfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int c;
    string plain_T;

    // Deobfuscation process
    for (size_t r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;

        // Swap section
        swap(s[i], s[j]);

        int tmp_index = (s[i] + s[j]) % 256;
        k = s[tmp_index];

        c = (static_cast<int>(bvn[r]) ^ k);  // XOR with the generated key
        plain_T += static_cast<char>(c);  // Convert int to char and append to output string
    }
    return plain_T;
}

string obfuscateBVN(vector<int> s, const vector<char>& key, const string& bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int c;
    string cipher_T;

    // Obfuscation process
    for (size_t r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;

        // Swap section
        swap(s[i], s[j]);

        int tmp_index = (s[i] + s[j]) % 256;
        k = s[tmp_index];

        c = (static_cast<int>(bvn[r]) ^ k);  // XOR the BVN character with the key
        cipher_T += static_cast<char>(c);  // Convert int to char and append to output string
    }

    return cipher_T;
}

vector<int> initializeSBox(vector<int> s, const vector<char>& key) {
    int b = 0;

    // Initialize the s-box with values from 0 to 255
    for (int a = 0; a < 256; a++) {
        s[a] = a;
    }

    // Perform the key scheduling to generate the s-box
    for (int a = 0; a < 256; a++) {
        b = (b + s[a] + key[a % key.size()]) % 256;
        swap(s[a], s[b]);
    }

    return s;
}