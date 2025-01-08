#include <iostream>
#include <string>
#include <vector>

using namespace std;

// RC4 encryption/decryption function (based on the Wikipedia pseudo-code)
void RC4(vector<unsigned char>& data, const string& key) {
    const size_t data_size = data.size();
    const size_t key_size = key.size();

    vector<unsigned char> S(256);
    unsigned char j = 0;

    // Initialization
    for (unsigned char i = 0; i < 256; ++i) {
        S[i] = i;
    }

    for (unsigned char i = 0; i < 256; ++i) {
        j = (j + S[i] + key[i % key_size]) % 256;
        swap(S[i], S[j]);
    }

    // Encryption/decryption
    j = 0;
    for (unsigned char i = 0; i < data_size; ++i) {
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        const unsigned char k = (S[i] + S[j]) % 256;
        data[i] ^= S[k];
    }
}

string obfuscateBVN(const string& bvn, const string& key) {
    vector<unsigned char> bvnData(bvn.begin(), bvn.end());

    RC4(bvnData, key);

    // Convert encrypted data back to string
    string obfuscatedBVN;
    for (const unsigned char& c : bvnData) {
        obfuscatedBVN.push_back(static_cast<char>(c));
    }

    return obfuscatedBVN;
}

string deObfuscateBVN(const string& obfuscatedBVN, const string& key) {
    vector<unsigned char> obfuscatedData(obfuscatedBVN.begin(), obfuscatedBVN.end());

    RC4(obfuscatedData, key);

    // Convert decrypted data back to string
    string deObfuscatedBVN;
    for (const unsigned char& c : obfuscatedData) {
        deObfuscatedBVN.push_back(static_cast<char>(c));
    }

    return deObfuscatedBVN;
}

int main() {
    string bvnToObfuscate = "1234567890";
    string encryptionKey = "your_secret_encryption_key";  // Replace this with a strong, random key

    // Obfuscate BVN
    string obfuscatedBVN = obfuscateBVN(bvnToObfuscate, encryptionKey);
    cout << "Obfuscated BVN: " << obfuscatedBVN << endl;

    // De-obfuscate BVN
    string deObfuscatedBVN = deObfuscateBVN(obfuscatedBVN, encryptionKey);
    cout << "De-obfuscated BVN: " << deObfuscatedBVN << endl;

    return 0;
}