#include <iostream>
#include <sstream>
#include <string>

class User {
public:
    User() = default;
    User(const std::string& name, const std::string& password)
        : name(name), password(password) {}

    // Serialize the object to a stream
    void serialize(std::ostream& os) const {
        os << name << '\n' << password << '\n';
    }

    // Deserialize the object from a stream
    void deserialize(std::istream& is) {
        std::getline(is, name);
        std::getline(is, password);
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
    
    // Serialization 
    std::ostringstream oss;
    user.serialize(oss);
    
    std::string serialized_data = oss.str();
    std::cout << "Serialized User Data:\n" << serialized_data;

    // Deserialization
    User new_user;
    std::istringstream iss(serialized_data);
    new_user.deserialize(iss);
    
    std::cout << "Deserialized User Data:\n";
    new_user.display();

    return 0;
}