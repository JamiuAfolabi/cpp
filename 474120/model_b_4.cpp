#include <iostream>
#include <string>
#include <cassert>
#include <bitset>
#include <sstream>

class XorEncryptor {
public:
    // Function to normalize the key to match the length of the plaintext
    std::string normalize_key(const std::string& plaintext, const std::string& key) const {
        std::string repeated_key = key;
        while (repeated_key.length() < plaintext.length()) {
            repeated_key += key;
        }
        return repeated_key.substr(0, plaintext.length());
    }

    // XOR Encryption function
    std::string xor_encrypt(const std::string& plaintext, const std::string& key) const {
        std::string encrypted = "";
        std::string repeated_key = normalize_key(plaintext, key);

        for (size_t i = 0; i < plaintext.length(); i++) {
            encrypted += char(plaintext[i] ^ repeated_key[i]);  // XOR each character with the key
        }
        return encrypted;
    }

    // XOR Decryption function (same as encryption)
    std::string xor_decrypt(const std::string& encrypted, const std::string& key) const {
        return xor_encrypt(encrypted, key);  // Decryption is the same as encryption with XOR
    }

    // Function to encode encrypted data in base64 for safe transport/storage
    std::string encode_base64(const std::string& data) const {
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

    // Function to decode base64 data back to a string
    std::string decode_base64(const std::string& encoded) const {
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
};

// Unit Tests
void run_tests() {
    XorEncryptor encryptor; // Create an instance of XorEncryptor

    // Test data
    std::string original_text = "Hello World!";
    std::string key = "my_secret_key";

    // Test 1: Check encryption and decryption
    std::string encrypted_text = encryptor.xor_encrypt(original_text, key);
    std::string decrypted_text = encryptor.xor_decrypt(encrypted_text, key);
    assert(original_text == decrypted_text && "Decryption failed: original and decrypted texts do not match.");

    // Test 2: Check whether encryption changes the text
    assert(original_text != encrypted_text && "Encryption failed: original and encrypted texts are the same.");

    // Test 3: Validate empty string handling
    std::string empty_text = "";
    std::string encrypted_empty = encryptor.xor_encrypt(empty_text, key);
    std::string decrypted_empty = encryptor.xor_decrypt(encrypted_empty, key);
    assert(empty_text == decrypted_empty && "Decryption of an empty string failed.");

    // Test 4: Check with a simple lowercase test
    std::string simple_text = "test";
    std::string simple_key = "1";
    std::string encrypted_simple = encryptor.xor_encrypt(simple_text, simple_key);
    std::string decrypted_simple = encryptor.xor_decrypt(encrypted_simple, simple_key);
    assert(simple_text == decrypted_simple && "Decryption of simple text with single character key failed.");

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    run_tests(); // Run the unit tests

    // Example usage of XorEncryptor:
    XorEncryptor encryptor;
    std::string original_text = "Hello World!";
    std::string key = "my_secret_key";

    // Encrypt the original text
    std::string encrypted_text = encryptor.xor_encrypt(original_text, key);
    std::cout << "Encrypted: " << encryptor.encode_base64(encrypted_text) << std::endl;

    // Decrypt the encrypted text
    std::string decrypted_text = encryptor.xor_decrypt(encrypted_text, key);
    std::cout << "Decrypted: " << decrypted_text << std::endl;

    return 0;
}