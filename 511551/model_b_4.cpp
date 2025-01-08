#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <CommonCrypto/CommonDigest.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

// Function to initialize the s-box using a provided key
vector<int> initializeSBox(vector<int> s, const vector<char>& key);

// Function to obfuscate a single batch of data
string obfuscateBatch(vector<int> s, const vector<char>& key, const vector<string>& batch);

// Function to deobfuscate a single batch of data
vector<string> deobfuscateBatch(vector<int> s, const vector<char>& key, const vector<string>& batch);

// Function to generate a random key of specified length
vector<char> generateRandomKey(size_t length);

// Function to hash a string using SHA-256 (CommonCrypto)
string hashSHA256(const string& input);

// Function to process batches of data in parallel using threads
void processBatches(vector<int> s, const vector<char>& key, const vector<vector<string>>& batches, vector<vector<string>>& deobfuscatedBatches, mutex& mtx, condition_variable& cv, bool& done);

int main() {
    const size_t batchSize = 100;  // Define the batch size
    size_t numBatches = 1000;  // Define the number of batches
    vector<int> S(256); // s-box initialization vector

    // Generate a random key of 16 bytes (128 bits)
    vector<char> key = generateRandomKey(16);

    // Initialize the s-box with the key
    S = initializeSBox(S, key);

    // Create vectors to hold batches of data and their corresponding deobfuscated results
    vector<vector<string>> batches(numBatches);
    vector<vector<string>> deobfuscatedBatches(numBatches);

    // Generate dummy data for testing purposes
    for (size_t i = 0; i < numBatches; ++i) {
        for (size_t j = 0; j < batchSize; ++j) {
            string bvn = "1234567890123456"; // Example BVN
            batches[i].push_back(bvn);
        }
    }

    // Create threads to process the batches in parallel
    mutex mtx;
    condition_variable cv;
    bool done = false;
    const int numThreads = 4;  // Define the number of threads
    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&] {
            processBatches(S, key, batches, deobfuscatedBatches, mtx, cv, done);
        });
    }

    // Wait for all threads to finish
    for (thread& t : threads) {
        t.join();
    }

    // Perform hash verification for each batch
    for (size_t i = 0; i < numBatches; ++i) {
        for (size_t j = 0; j < batchSize; ++j) {
            string originalBvn = batches[i][j];
            string deobfuscatedBvn = deobfuscatedBatches[i][j];

            string originalHash = hashSHA256(originalBvn);
            string deobfuscatedHash = hashSHA256(deobfuscatedBvn);

            bool isVerified = compareHashes(originalHash, deobfuscatedHash);

            // Print the results
            cout << "Original BVN: " << originalBvn << endl;
            cout << "Deobfuscated BVN: " << deobfuscatedBvn << endl;
            cout << "Hashes match: " << (isVerified ? "Yes" : "No") << endl;
        }
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

string obfuscateBatch(vector<int> s, const vector<char>& key, const vector<string>& batch) {
    vector<string> obfuscatedBatch;
    for (const string& bvn : batch) {
        string obfuscatedBVN = obfuscateBVN(s, key, bvn);
        obfuscatedBatch.push_back(obfuscatedBVN);
    }
    return obfuscatedBatch;
}

vector<string> deobfuscateBatch(vector<int> s, const vector<char>& key, const vector<string>& batch) {
    vector<string> deobfuscatedBatch;
    for (const string& obfuscatedBVN : batch) {
        string deobfuscatedBVN = deobfuscateBVN(s, key, obfuscatedBVN);
        deobfuscatedBatch.push_back(deobfuscatedBVN);
    }
    return deobfuscatedBatch;
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

void processBatches(vector<int> s, const vector<char>& key, const vector<vector<string>>& batches, vector<vector<string>>& deobfuscatedBatches, mutex& mtx, condition_variable& cv, bool& done) {
    while (!done) {
        vector<string> batch;
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&] { return !batches.empty() || done; });

            if (done) {
                return;
            }

            batch = move(batches.back());
            batches.pop_back();
        }

        vector<string> deobfuscatedBatch = deobfuscateBatch(s, key, batch);
        {
            unique_lock<mutex> lock(mtx);
            deobfuscatedBatches.push_back(move(deobfuscatedBatch));
            cv.notify_one();
        }
    }
}