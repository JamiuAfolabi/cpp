#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>  // Add this header for std::vector
#include <cassert>

// Function to normalize the key to match the length of the plaintext
std::string normalize_key(const std::string& plaintext, const std::string& key) {
    std::string repeated_key = key;
    while (repeated_key.length() < plaintext.length()) {
        repeated_key += key;
    }
    return repeated_key.substr(0, plaintext.length());
}

// XOR Encryption function
std::string xor_encrypt(const std::string& plaintext, const std::string& key) {
    std::string encrypted = "";
    std::string repeated_key = normalize_key(plaintext, key);
    
    for (size_t i = 0; i < plaintext.length(); i++) {
        encrypted += char(plaintext[i] ^ repeated_key[i]);  // XOR each character with the key
    }
    return encrypted;
}

// XOR Decryption function (same as encryption)
std::string xor_decrypt(const std::string& encrypted, const std::string& key) {
    return xor_encrypt(encrypted, key);  // Decryption is the same as encryption with XOR
}

// Function to encode encrypted data in base64 for safe transport/storage
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

// Function to decode base64 data back to a string
std::string decode_base64(const std::string& encoded) {
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    std::vector<int> T(256, -1);  // Now it can recognize std::vector
    
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

int main() {
    // Original text and key for testing
    std::string original_text = "Hello World";
    std::string key = "my_secrets_key";

    // Encrypt and decrypt the original text
    std::string encrypted_text = xor_encrypt(original_text, key);
    std::string decrypted_text = xor_decrypt(encrypted_text, key);

    // Run unit tests
    run_tests(original_text, key, encrypted_text, decrypted_text);

    // Encrypt and base64 encode the original text
    encrypted_text = xor_encrypt(original_text, key);
    std::string base64_encrypted = encode_base64(encrypted_text);

    // Decrypt and base64 decode the encrypted text
    decrypted_text = xor_decrypt(encrypted_text, key);
    std::string base64_decrypted = decode_base64(base64_encrypted);

    std::cout << "Encrypted (Base64): " << base64_encrypted << std::endl;
    std::cout << "Decrypted (Base64): " << base64_decrypted << std::endl;

    return 0;
}

void run_tests(const std::string& original_text, const std::string& key, const std::string& encrypted_text, const std::string& decrypted_text) {
    std::cout << "Running unit tests..." << std::endl;

    // Test 1: Encryption
    std::string encrypted_test = xor_encrypt(original_text, key);
    assert(encrypted_test == encrypted_text, "Encryption failed");

    // Test 2: Decryption
    std::string decrypted_test = xor_decrypt(encrypted_text, key);
    assert(decrypted_test == decrypted_text, "Decryption failed");

    // Test 3: Original text vs decrypted text
    assert(decrypted_text == original_text, "Original text not recovered after decryption");

    std::cout << "All tests passed!" << std::endl;
}