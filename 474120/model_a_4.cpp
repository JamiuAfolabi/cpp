#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>

class XOREncryptor {
public:
    XOREncryptor(const std::string& key) : key_(key) {}

    // Encrypt the given plaintext
    std::string encrypt(const std::string& plaintext) {
        std::string encrypted = "";
        std::string repeated_key = normalize_key(plaintext);
        
        for (size_t i = 0; i < plaintext.length(); i++) {
            encrypted += char(plaintext[i] ^ repeated_key[i]);  // XOR each character with the key
        }
        return encrypted;
    }

    // Decrypt the given ciphertext
    std::string decrypt(const std::string& ciphertext) {
        return encrypt(ciphertext);  // Decryption is the same as encryption with XOR
    }

    // Encode the data in base64 for safe transport/storage
    std::string encode_base64(const std::string& data) {
        static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string encoded;
        int val = 0, valb = -6;

        for (unsigned char c : data) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }

        if (valb > -6) {
            encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        }

        while (encoded.size() % 4) {
            encoded.push_back('=');
        }

        return encoded;
    }

    // Decode base64 data back to a string
    std::string decode_base64(const std::string& encoded) {
        static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string decoded;
        std::vector<int> T(256, -1);

        for (int i = 0; i < 64; i++) {
            T[base64_chars[i]] = i;
        }

        int val = 0, valb = -8;
        for (unsigned char c : encoded) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                decoded.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return decoded;
    }

private:
    std::string key_;

    // Normalize the key to match the length of the plaintext
    std::string normalize_key(const std::string& plaintext) {
        std::string repeated_key = key_;
        while (repeated_key.length() < plaintext.length()) {
            repeated_key += key_;
        }
        return repeated_key.substr(0, plaintext.length());
    }
};

// Unit Tests
void run_tests() {
    // Test data
    std::string original_text = "Hello World!";
    std::string key = "my_secret_key";
    XOREncryptor encryptor(key);

    // Test 1: Check encryption and decryption
    std::string encrypted_text = encryptor.encrypt(original_text);
    std::string decrypted_text = encryptor.decrypt(encrypted_text);
    assert(original_text == decrypted_text && "Decryption failed: original and decrypted texts do not match.");

    // Test 2: Check whether encryption changes the text
    assert(original_text != encrypted_text && "Encryption failed: original and encrypted texts are the same.");

    // Test 3: Validate empty string handling
    std::string empty_text = "";
    std::string encrypted_empty = encryptor.encrypt(empty_text);
    std::string decrypted_empty = encryptor.decrypt(encrypted_empty);
    assert(empty_text == decrypted_empty && "Decryption of an empty string failed.");

    // Test 4: Check with a simple lowercase test
    std::string simple_text = "test";
    std::string simple_key = "1";
    XOREncryptor simple_encryptor(simple_key);
    std::string encrypted_simple = simple_encryptor.encrypt(simple_text);
    std::string decrypted_simple = simple_encryptor.decrypt(encrypted_simple);
    assert(simple_text == decrypted_simple && "Decryption of simple text with single character key failed.");

    std::cout << "All tests passed!" << std::endl;
}

// Main function
int main() {
    run_tests(); // Run the unit tests

    // Example usage:
    std::string original_text = "Hello World!";
    std::string key = "my_secret_key";
    XOREncryptor encryptor(key);

    // Encrypt the original text
    std::string encrypted_text = encryptor.encrypt(original_text);
    std::cout << "Encrypted: " << encryptor.encode_base64(encrypted_text) << std::endl;

    // Decrypt the encrypted text
    std::string decrypted_text = encryptor.decrypt(encrypted_text);
    std::cout << "Decrypted: " << decrypted_text << std::endl;

    return 0;
}