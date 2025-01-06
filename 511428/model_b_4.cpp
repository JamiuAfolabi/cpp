#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <thread>
#include <vector>
#include <future>

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


// Function to serialize a single User object
std::string serializeUser(const User& user, std::function<bool(const std::string&, const std::string&)> validate) {
    std::ostringstream oss;
    try {
        user.serialize(oss, validate);
        return oss.str();
    } catch (const std::exception& e) {
        std::cerr << "Error serializing user: " << e.what() << '\n';
        return "";
    }
}

// Function to perform parallel serialization
std::vector<std::string> parallelSerialize(const std::vector<User>& users, size_t numThreads, std::function<bool(const std::string&, const std::string&)> validate) {
    std::vector<std::future<std::string>> results;
    size_t chunkSize = users.size() / numThreads + 1;

    for (size_t threadId = 0; threadId < numThreads; ++threadId) {
        size_t start = threadId * chunkSize;
        size_t end = std::min((threadId + 1) * chunkSize, users.size());
        results.push_back(std::async(std::launch::async, [start, end, &users, validate] {
            std::stringstream serializedData;
            for (size_t i = start; i < end; ++i) {
                serializedData << serializeUser(users[i], validate) << '\n';
            }
            return serializedData.str();
        }));
    }

    std::vector<std::string> serializedDatas;
    for (auto& result : results) {
        serializedDatas.push_back(result.get());
    }

    return serializedDatas;
}

int main() {
    // ... (Same as before, create users, validate lambda, etc.)

    // Example of a large dataset
    std::vector<User> largeUsers = {
        User("Alice", "Password123"),
        User("Bob", "Password456"),
        User("Charlie", "Password789"),
        // Add more users as needed
    };

    auto validateUser = [](const std::string& name, const std::string& password) {
        return !name.empty() && password.length() >= 8; // Example validation
    };

    const size_t numThreads = 4; // Adjust the number of threads based on your system's capabilities

    // Parallel serialization
    std::vector<std::string> parallelSerializedData = parallelSerialize(largeUsers, numThreads, validateUser);

    // Now you have a vector of strings containing serialized data for each User object

    // You can then save this parallelSerializedData to a file or perform further operations.

    return 0;
}