#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <random>
#include <unordered_map>
#include <mutex>

#define SERVER_HEALTH_CHECK_INTERVAL 2  // Health check interval in seconds

// Server class
class Server {
public:
    Server(int id) : id(id), health(true) {}
    int id;
    bool health;
};

// Load Balancer class
class LoadBalancer {
public:
    LoadBalancer() : current_server_index(0) {}

    // Server Pool Management
    void addServer(int id) {
        std::unique_lock<std::mutex> lock(mutex);
        servers.push_back(Server(id));
    }

    void removeServer(int id) {
        std::unique_lock<std::mutex> lock(mutex);
        for (size_t i = 0; i < servers.size(); ++i) {
            if (servers[i].id == id) {
                servers.erase(servers.begin() + i);
                return;
            }
        }
    }

    // Health Monitoring
    void healthMonitorThread() {
        while (true) {
            {
                std::unique_lock<std::mutex> lock(mutex);
                for (auto& server : servers) {
                    // Simulate health check
                    bool randomHealth = std::uniform_int_distribution<>(0, 1)(rand_engine) == 0;
                    server.health = randomHealth;
                }
            }
            healthCheckCV.notify_all();
            std::this_thread::sleep_for(std::chrono::seconds(SERVER_HEALTH_CHECK_INTERVAL));
        }
    }

    // Round Robin Scheduling
    int getNextServer() {
        std::unique_lock<std::mutex> lock(mutex);
        healthCheckCV.wait(lock);  // Wait for health check to complete

        do {
            while (current_server_index >= servers.size()) {
                current_server_index = 0;
            }

            while (!servers[current_server_index].health) {
                ++current_server_index;
            }
        } while (current_server_index >= servers.size());

        int selectedServer = servers[current_server_index].id;
        ++current_server_index;
        return selectedServer;
    }

private:
    std::vector<Server> servers;
    size_t current_server_index = 0;

    std::mutex mutex;
    std::condition_variable healthCheckCV;
    std::random_device rd;
    std::mt19937 rand_engine{rd()};
};

int main() {
    LoadBalancer lb;

    // Start health monitoring thread
    std::thread healthMonitorThread(&LoadBalancer::healthMonitorThread, &lb);

    // Add some servers
    lb.addServer(1);
    lb.addServer(2);
    lb.addServer(3);

    // Simulate traffic distribution
    for (int i = 0; i < 10; ++i) {
        int selectedServer = lb.getNextServer();
        std::cout << "Sending request to Server " << selectedServer << std::endl;
    }

    // Remove a server
    lb.removeServer(2);

    // Continue simulating traffic distribution
    for (int i = 0; i < 10; ++i) {
        int selectedServer = lb.getNextServer();
        std::cout << "Sending request to Server " << selectedServer << std::endl;
    }

    healthMonitorThread.join();
    return 0;
}
