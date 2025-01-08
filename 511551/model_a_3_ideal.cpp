#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <CommonCrypto/CommonDigest.h>  // Include the CommonCrypto library
#include <sstream>  // Include the stringstream header
#include <iomanip>  // Include the iomanip header for formatting

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

int main() {
    string bvn = "1234567890123456";  // Example BVN
    vector<int> S(256); // s-box initialization vector

    // Generate a random key of 16 bytes (128 bits)
    vector<char> key = generateRandomKey(16);

    // Initialize the s-box with the key
    S = initializeSBox(S, key);

    // Obfuscate and deobfuscate the BVN
    string obfuscatedBVN = obfuscateBVN(S, key, bvn);
    string deobfuscatedBVN = deobfuscateBVN(S, key, obfuscatedBVN);

    // Hash the original and deobfuscated BVN for verification
    string originalHash = hashSHA256(bvn);
    string deobfuscatedHash = hashSHA256(deobfuscatedBVN);

    // Compare the original and deobfuscated hashes
    bool isVerified = compareHashes(originalHash, deobfuscatedHash);

    // Print the results
    cout << "Original BVN: " << bvn << endl;
    cout << "Obfuscated BVN: " << obfuscatedBVN << endl;
    cout << "Deobfuscated BVN: " << deobfuscatedBVN << endl;
    cout << "Hashes match: " << (isVerified ? "Yes" : "No") << endl;

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

string hashSHA256(const string& input) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(input.c_str(), static_cast<CC_LONG>(input.length()), hash);

    // Convert the hash to a hexadecimal string
    stringstream hexHash;
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) {
        hexHash << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return hexHash.str();
}

bool compareHashes(const string& originalHash, const string& newHash) {
    return originalHash == newHash;
}
