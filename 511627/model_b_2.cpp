#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Function to initialize the s-box using a provided key
vector<int> initializeSBox(vector<int> s, vector<int> key);

// Function to obfuscate the BVN (Bank Verification Number)
string obfuscateBVN(vector<int> s, vector<int> key, string bvn);

// Function to deobfuscate the BVN
string deobfuscateBVN(vector<int> s, vector<int> key, string bvn);

int main() {

    string bvn = "1234567890123456";  // Example BVN
    vector<int> S(256); // s-box initialization vector
    vector<int> key = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40
    }; // Example key of length 40 bytes

    // Initialize the s-box with the key
    S = initializeSBox(S, key);

    // Obfuscate and deobfuscate the BVN
    string obfuscatedBVN = obfuscateBVN(S, key, bvn);
    string deobfuscatedBVN = deobfuscateBVN(S, key, obfuscatedBVN);

    // Print the results
    cout << "Original BVN: " << bvn << endl;
    cout << "Obfuscated BVN: " << obfuscatedBVN << endl;
    cout << "Deobfuscated BVN: " << deobfuscatedBVN << endl;

    return 0;
}

string deobfuscateBVN(vector<int> s, vector<int> key, string bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int b;
    int c;
    int * plain = new int[bvn.length()];
    string plain_T;

    // Deobfuscation process
    for (int r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;

        // Swap section
        b = s[i];
        s[i] = s[j];
        s[j] = b;

        tmp = (s[i] + s[j]) % 256;
        k = s[tmp];

        c = ((int)bvn[r] ^ k);  // XOR with the generated key

        plain[r] = c;

        plain_T += (char)plain[r];  // Convert int array to char and append to the output string
    }
    return plain_T;
}

string obfuscateBVN(vector<int> s, vector<int> key, string bvn) {
    int i = 0, j = 0, tmp = 0, k = 0;
    int b;
    int c;
    int * cipher = new int[bvn.length()];
    string cipher_T;

    // Obfuscation process
    for (int r = 0; r < bvn.length(); r++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;

        // Swap section
        b = s[i];
        s[i] = s[j];
        s[j] = b;

        tmp = (s[i] + s[j]) % 256;
        k = s[tmp];

        c = ((int)bvn[r] ^ k);  // XOR the BVN character with the key

        cipher[r] = c;

        cipher_T += (char)cipher[r];  // Convert int to char and append to the output string
    }

    return cipher_T;
}

vector<int> initializeSBox(vector<int> s, vector<int> key) {
    int b = 0, tmp;
    int keyIndex = 0;

    // Initialize the s-box with values from 0 to 255
    for (int a = 0; a < 256; a++) {
        s[a] = a;
    }

    // Perform the key scheduling to generate the s-box
    for (int a = 0; a < 256; a++) {
        b = (b + s[a] + key[keyIndex % key.size()]) % 256;
        keyIndex++;
        tmp = s[a];
        s[a] = s[b];
        s[b] = tmp;
    }

    return s;
}