#include <iostream>
#include <string>
#include <vector>
#include <bitset>

// Function to normalize the key for the plaintext
std::string normalize_key(const std::string& plaintext, const std::string& key) {
    std::string repeated_key = key;
    while (repeated_key.length() < plaintext.length()) {
        repeated_key += key;
    }
    return repeated_key.substr(0, plaintext.length());
}

// XOR encryption function
std::string xor_encrypt(const std::string& plaintext, const std::string& key) {
    std::string encrypted;
    std::string normalized_key = normalize_key(plaintext, key);
    
    for (size_t i = 0; i < plaintext.size(); i++) {
        // XOR each character and convert it to char
        encrypted += static_cast<char>(plaintext[i] ^ normalized_key[i]);
    }
    return encrypted;
}

// XOR decryption function (same as encryption)
std::string xor_decrypt(const std::string& encrypted, const std::string& key) {
    return xor_encrypt(encrypted, key); // Decrypting is the same as encrypting for XOR
}

// Base64 encoding helper functions
std::string base64_encode(const std::string& in) {
    static const char* base64_table =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string out;
    const unsigned char* bytes_to_encode = reinterpret_cast<const unsigned char*>(in.c_str());
    unsigned int in_len = in.size();

    for (unsigned int i = 0; i < in_len;) {
        unsigned int char_array_3[3] = { 0, 0, 0 };
        unsigned int char_array_4[4] = { 0, 0, 0, 0 };

        for (int j = 0; j < 3 && i < in_len; j++, i++)
            char_array_3[j] = bytes_to_encode[i];

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; (j < 4) && (j < (in_len + 3) / 3); j++)
            out += base64_table[char_array_4[j]];
        
        while ((in_len % 3) && (++j < 4))
            out += '=';
    }
    return out;
}

std::string base64_decode(const std::string& in) {
    static constexpr unsigned char d[] = {
        62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57,
        58, 59, 60, 61, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 
        255, 255, 255, 255, 255, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
        43, 44, 45, 46, 47, 48, 49, 50, 51
    };

    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[(unsigned char)base64_table[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out += (char)((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return out;
}

int main() {
    std::string original_text = "Hello World!";
    std::string key = "my_secure_key";

    // Encrypt
    std::string encrypted_text = xor_encrypt(original_text, key);
    std::string base64_encrypted_text = base64_encode(encrypted_text);
    std::cout << "Encrypted (Base64): " << base64_encrypted_text << std::endl;

    // Decrypt
    std::string decrypted_text = xor_decrypt(encrypted_text, key);
    std::cout << "Decrypted: " << decrypted_text << std::endl;

    return 0;
}