#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>

class User {
public:
    User() = default;
    User(const std::string& name, const std::string& password)
        : name(name), password(password) {}

    void serialize(std::ostream& os) const {
        os << name << '\n' << password << '\n';
    }

    void deserialize(std::istream& is) {
        std::getline(is, name);
        std::getline(is, password);
    }

private:
    std::string name;
    std::string password;
};

// Serialize multiple users in parallel
void serializeUsers(const std::vector<User>& users, std::ostream& os, std::mutex& mtx) {
    for (const auto& user : users) {
        std::lock_guard<std::mutex> lock(mtx); // Locking to ensure thread safety
        user.serialize(os);
    }
}

// Deserialize multiple users in parallel
void deserializeUsers(std::istream& is, std::vector<User>& users, std::mutex& mtx, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        User user;
        user.deserialize(is);
        std::lock_guard<std::mutex> lock(mtx); // Locking to ensure thread safety
        users.push_back(user);
    }
}

int main() {
    std::vector<User> users = {
        User("Alice", "Password123"),
        User("Bob", "Password456"),
        User("Charlie", "Password789"),
        // Add more users as needed
    };

    // Serialize users
    std::ostringstream oss;
    std::mutex mtx;
    std::vector<std::thread> threads;

    size_t numThreads = std::thread::hardware_concurrency(); // Get number of concurrent threads supported
    size_t chunkSize = users.size() / numThreads;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? users.size() : (i + 1) * chunkSize;

        threads.emplace_back(serializeUsers, std::ref(std::vector<User>(users.begin() + start, users.begin() + end)), std::ref(oss), std::ref(mtx));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    std::string serialized_data = oss.str();
    std::cout << "Serialized User Data:\n" << serialized_data;

    // Deserialize users
    std::istringstream iss(serialized_data);
    std::vector<User> new_users;
    threads.clear();
    size_t userCount = users.size();

    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(deserializeUsers, std::ref(iss), std::ref(new_users), std::ref(mtx), userCount / numThreads);
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Deserialized User Data:\n";
    for (const auto& user : new_users) {
        // Display or process deserialized users
    }

    return 0;
}