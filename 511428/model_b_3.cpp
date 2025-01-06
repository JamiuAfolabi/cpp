#include <iostream>
#include <sstream>
#include <string>
#include <functional>

class User {
public:
    using SerializeValidation = std::function<void(const std::string&, const std::string&)>;
    using DeserializeValidation = std::function<bool(const std::string&, const std::string&)>;

    User() = default;
    User(const std::string& name, const std::string& password)
        : name(name), password(password) {}

    // Setter for serialization validation
    void setSerializeValidation(SerializeValidation validation) { serializeValidation = validation; }

    // Setter for deserialization validation
    void setDeserializeValidation(DeserializeValidation validation) { deserializeValidation = validation; }

    // Serialize the object to a stream with optional validation
    void serialize(std::ostream& os) const {
        os << name << '\n' << password << '\n';
        if (serializeValidation) {
            serializeValidation(name, password);
        }
    }

    // Deserialize the object from a stream with optional validation
    bool deserialize(std::istream& is) {
        std::getline(is, name);
        std::getline(is, password);

        if (deserializeValidation && !deserializeValidation(name, password)) {
            std::cerr << "Validation failed during deserialization.\n";
            return false;
        }
        return true;
    }

    void display() const {
        std::cout << "Name: " << name << "\nPassword: " << password << '\n';
    }

private:
    std::string name;
    std::string password;

    SerializeValidation serializeValidation;
    DeserializeValidation deserializeValidation;
};

int main() {
    User user("Alice", "SecurePassword123");

    // Example of validation: Ensure username has 3 or more characters
    auto usernameLengthValidation = [](const std::string& name, const std::string&) {
        if (name.length() < 3) {
            std::cerr << "Validation error: Username must have at least 3 characters.\n";
        }
    };

    // Example of validation: Check if password has an uppercase letter
    auto passwordUppercaseValidation = [](const std::string&, const std::string& password) {
        for (char ch : password) {
            if (std::isupper(ch)) {
                return true;
            }
        }
        std::cerr << "Validation error: Password must contain an uppercase letter.\n";
        return false;
    };

    // Serialization with validation
    user.setSerializeValidation(usernameLengthValidation);
    std::ostringstream oss;
    user.serialize(oss);

    // Deserialization with validation
    User new_user;
    new_user.setDeserializeValidation(passwordUppercaseValidation);
    std::istringstream iss(oss.str());
    if (!new_user.deserialize(iss)) {
        return 1;
    }

    new_user.display();

    return 0;
}