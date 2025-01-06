#include <iostream>
#include <sstream>
#include <string>
#include <functional>

class User {
public:
    User() = default;
    User(const std::string& name, const std::string& password)
        : name(name), password(password) {}

    // Serialize the object to a stream with validation using a lambda function
    void serialize(std::ostream& os, std::function<bool(const std::string&, const std::string&)> validate) const {
        if (validate(name, password)) {
            os << name << '\n' << password << '\n';
        } else {
            throw std::invalid_argument("Validation failed during serialization.");
        }
    }

    // Deserialize the object from a stream with validation using a lambda function
    void deserialize(std::istream& is, std::function<bool(const std::string&, const std::string&)> validate) {
        std::getline(is, name);
        std::getline(is, password);
        
        if (!validate(name, password)) {
            throw std::invalid_argument("Validation failed during deserialization.");
        }
    }

    void display() const {
        std::cout << "Name: " << name << "\nPassword: " << password << '\n';
    }

private:
    std::string name;     // sensitive data
    std::string password; // sensitive data
};

int main() {
    User user("Alice", "SecurePassword123");

    // Define validation criteria using a lambda function
    auto validateUser = [](const std::string& name, const std::string& password) {
        return !name.empty() && password.length() >= 8; // Example validation
    };

    // Serialization
    std::ostringstream oss;
    try {
        user.serialize(oss, validateUser);
        std::string serialized_data = oss.str();
        std::cout << "Serialized User Data:\n" << serialized_data;

        // Deserialization
        User new_user;
        std::istringstream iss(serialized_data);
        new_user.deserialize(iss, validateUser);
        
        std::cout << "Deserialized User Data:\n";
        new_user.display();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}