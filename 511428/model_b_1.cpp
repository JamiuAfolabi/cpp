#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct UserData {
    std::string username;
    std::string email;
    std::vector<std::string> passwords;

    // Serialization function
    void serialize(std::ostringstream& oss) const {
        oss << username << " ";
        oss << email << " ";
        std::copy(passwords.begin(), passwords.end(), std::ostreambuf_iterator<char>(oss, ' '));
    }

    // Deserialization function
    void deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::getline(iss, username, ' ');
        std::getline(iss, email, ' ');
        std::string password;
        while (std::getline(iss, password, ' ')) {
            passwords.push_back(password);
        }
    }
};

class SensitiveUserDataHandler {
public:
    // Encrypt/Serialize user data
    std::string serialize(const UserData& data) const {
        std::ostringstream oss;
        data.serialize(oss);
        return oss.str();
    }

    // Decrypt/Deserialize user data
    UserData deserialize(const std::string& data) const {
        UserData userData;
        userData.deserialize(data);
        return userData;
    }

private:
    // Simple encryption/decryption using ROT13 (for demonstration purposes only, use proper encryption in production)
    static char encrypt(char ch) {
        if (std::isalpha(ch)) {
            ch = (ch - 'a' + 13) % 26 + 'a';
        }
        return ch;
    }

    static char decrypt(char ch) {
        return encrypt(ch);
    }

    std::string encryptData(const std::string& data) const {
        std::string encryptedData;
        for (char ch : data) {
            encryptedData.push_back(encrypt(ch));
        }
        return encryptedData;
    }

    std::string decryptData(const std::string& data) const {
        return encryptData(data);
    }
};

int main() {
    UserData userData;
    userData.username = "alice";
    userData.email = "alice@example.com";
    userData.passwords = { "secretpassword1", "secretpassword2" };

    SensitiveUserDataHandler handler;

    // Serialize and encrypt the user data
    std::string encryptedData = handler.serialize(userData);
    encryptedData = handler.encryptData(encryptedData);

    // Display encrypted data (for demonstration)
    std::cout << "Encrypted data: " << encryptedData << std::endl;

    // Decrypt and deserialize the user data
    UserData deserializedData = handler.deserialize(handler.decryptData(encryptedData));

    // Display deserialized data
    std::cout << "Deserialized username: " << deserializedData.username << std::endl;
    std::cout << "Deserialized email: " << deserializedData.email << std::endl;
    for (const std::string& password : deserializedData.passwords) {
        std::cout << "Deserialized password: " << password << std::endl;
    }

    return 0;
}
