#include <iostream>
#include <string>
#include <vector>

class RC4 {
public:
    RC4(const std::string& key) {
        initialize(key);
    }

    std::string encrypt(const std::string& plaintext) {
        return process(plaintext);
    }

    std::string decrypt(const std::string& ciphertext) {
        return process(ciphertext);
    }

private:
    std::vector<int> S;
    int i, j;

    void initialize(const std::string& key) {
        S.resize(256);
        for (int i = 0; i < 256; ++i) {
            S[i] = i;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % key.size()]) % 256;
            std::swap(S[i], S[j]);
        }

        this->i = 0;
        this->j = 0;
    }

    std::string process(const std::string& input) {
        std::string output;
        output.resize(input.size());

        for (size_t n = 0; n < input.size(); ++n) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);

            int K = S[(S[i] + S[j]) % 256];
            output[n] = input[n] ^ K;
        }

        return output;
    }
};

int main() {
    std::string key = "mysecretkey"; // Use a strong key
    std::string bvn = "12345678901";  // Example BVN

    // Initialize RC4 with the encryption key
    RC4 rc4(key);

    // Encrypt the BVN
    std::string encryptedBVN = rc4.encrypt(bvn);
    std::cout << "Encrypted BVN: ";
    for (unsigned char c : encryptedBVN) {
        std::cout << std::hex << static_cast<int>(c);
    }
    std::cout << std::dec << std::endl;

    // Decrypt the BVN
    std::string decryptedBVN = rc4.decrypt(encryptedBVN);
    std::cout << "Decrypted BVN: " << decryptedBVN << std::endl;

    return 0;
}