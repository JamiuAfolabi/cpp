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
        inline void write_string(std::ostream& os, const std::string& str) {
            os << str << '\n';
        }

        write_string(os, name);
        write_string(os, password);
    }

    // Deserialize the object from a stream
    void deserialize(std::istream& is) {
        inline void read_string(std::istream& is, std::string& str) {
            std::getline(is, str);
        }

        read_string(is, name);
        read_string(is, password);
    }

    void display() const {
        std::cout << "Name: " << name << "\nPassword: " << password << '\n';
    }

private:
    std::string name;
    std::string password;
};