#include <iostream>
#include <string>
#include <cstdint>  // For uint8_t
#include <stdexcept>
#include <cstdio>

// Step 3: Normalize the key to match the length of the plaintext
std::string normalizeKey(const std::string& plaintext, const std::string& key) {
    size_t keyLength = key.size();
    size_t plaintextLength = plaintext.size();
    size_t repeats = plaintextLength / keyLength + (plaintextLength % keyLength > 0 ? 1 : 0);

    std::string repeatedKey = key;
    repeatedKey.resize(repeats * keyLength);
    for (size_t i = keyLength; i < repeatedKey.size(); ++i) {
        repeatedKey[i] = key[i % keyLength];
    }

    return repeatedKey;
}

// Step 4: XOR encryption function
std::string xorEncrypt(const std::string& plaintext, const std::string& key) {
    std::string encrypted;
    std::string repeatedKey = normalizeKey(plaintext, key);

    if (plaintext.size() != repeatedKey.size()) {
        throw std::runtime_error("Error: Normalized key does not match plaintext length.");
    }

    for (size_t i = 0; i < plaintext.size(); ++i) {
        uint8_t plainChar = static_cast<uint8_t>(plaintext[i]);
        uint8_t keyChar = static_cast<uint8_t>(repeatedKey[i]);
        uint8_t encryptedChar = plainChar ^ keyChar;
        encrypted.push_back(static_cast<char>(encryptedChar));
    }

    return encrypted;
}

// Step 5: XOR decryption function (same as encryption)
std::string xorDecrypt(const std::string& encrypted, const std::string& key) {
    return xorEncrypt(encrypted, key);
}

// Function to handle non-printable characters (base64 encoding/decoding not implemented in this basic example)
// For simplicity, we'll just print a warning and handle non-printable characters as is.
void handleNonPrintable(const std::string& data) {
    for (char ch : data) {
        if (!isprint(ch)) {
            std::cerr << "Warning: Encrypted data contains non-printable character: " << ch << std::endl;
        }
    }
}

int main() {
    std::string originalText;
    std::cout << "Enter original text: ";
    std::getline(std::cin, originalText);

    std::string key;
    std::cout << "Enter encryption key: ";
    std::getline(std::cin, key);

    // Encrypt
    std::string encryptedText = xorEncrypt(originalText, key);

    // Handle non-printable characters in encrypted text
    handleNonPrintable(encryptedText);

    // Decrypt
    std::string decryptedText = xorDecrypt(encryptedText, key);

    std::cout << "Encrypted text: " << encryptedText << std::endl;
    std::cout << "Decrypted text: " << decryptedText << std::endl;

    return 0;
}
