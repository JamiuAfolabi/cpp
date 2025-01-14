#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>

struct Server {
    std::string id;
    bool isHealthy;

    Server(const std::string& id) : id(id), isHealthy(true) {}
};

// ServerPool manages a list of servers.
class ServerPool {
private:
    std::vector<Server> servers;
    std::mutex mtx; // To protect shared resources

public:
    void addServer(const std::string& serverId) {
        std::lock_guard<std::mutex> lock(mtx);
        servers.emplace_back(serverId);
    }

    void removeServer(const std::string& serverId) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = std::remove_if(servers.begin(), servers.end(), [&](const Server& server) {
            return server.id == serverId;
        });
        servers.erase(it, servers.end());
    }

    void markServerHealth(const std::string& serverId, bool healthStatus) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& server : servers) {
            if (server.id == serverId) {
                server.isHealthy = healthStatus;
            }
        }
    }

    std::vector<Server> getHealthyServers() {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<Server> healthyServers;
        for (const auto& server : servers) {
            if (server.isHealthy) {
                healthyServers.push_back(server);
            }
        }
        return healthyServers;
    }
};

void healthCheck(ServerPool& pool) {
    while (true) {
        // Simulating health check with dummy logic; replace with actual check logic
        std::this_thread::sleep_for(std::chrono::seconds(10));
        pool.markServerHealth("server1", true); // Just an example
        // Add actual health check logic here 
    }
}

class LoadBalancer {
private:
    ServerPool serverPool;
    std::atomic<size_t> currentIndex;

public:
    LoadBalancer() : currentIndex(0) {}

    void addServer(const std::string& serverId) {
        serverPool.addServer(serverId);
    }

    void removeServer(const std::string& serverId) {
        serverPool.removeServer(serverId);
    }

    std::string getNextServer() {
        auto healthyServers = serverPool.getHealthyServers();
        if (healthyServers.empty()) {
            throw std::runtime_error("No healthy servers available");
        } 

        // Round Robin algorithm to get the next server index 
        size_t index = currentIndex++ % healthyServers.size();
        return healthyServers[index].id;
    }
};

void simulateTraffic(LoadBalancer& lb) {
    for (int i = 0; i < 100; ++i) { // Simulate 100 requests
        try {
            std::string server = lb.getNextServer();
            std::cout << "Request directed to: " << server << std::endl;
        } catch(const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate time delay
    }
}

int main() {
    LoadBalancer lb;

    // Add servers
    lb.addServer("server1");
    lb.addServer("server2");
    lb.addServer("server3");

    // Start health check routine
    std::thread healthCheckThread(healthCheck, std::ref(lb));

    // Simulate traffic
    simulateTraffic(lb);

    // Clean up
    healthCheckThread.join();
    return 0;
}