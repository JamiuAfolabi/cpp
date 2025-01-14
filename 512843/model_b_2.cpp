#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <unordered_map>

struct Server {
    std::string id;
    bool isHealthy;
    int weight = 1; // Default weight to 1 if not specified

    Server(const std::string& id) : id(id), isHealthy(true) {}
};

// ServerPool manages a list of servers with their weights.
class ServerPool {
private:
    std::vector<Server> servers;
    std::unordered_map<std::string, int> serverWeights;
    std::mutex mtx; // To protect shared resources

public:
    void addServer(const std::string& serverId, int weight = 1) {
        std::lock_guard<std::mutex> lock(mtx);
        servers.emplace_back(serverId);
        serverWeights[serverId] = weight;
    }

    void removeServer(const std::string& serverId) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = std::remove_if(servers.begin(), servers.end(), [&](const Server& server) {
            return server.id == serverId;
        });
        servers.erase(it, servers.end());
        serverWeights.erase(serverId);
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

    int getTotalWeight() {
        std::lock_guard<std::mutex> lock(mtx);
        return std::accumulate(serverWeights.begin(), serverWeights.end(), 0, [](int total, const auto& entry) {
            return total + entry.second;
        });
    }

    // Getter for the server pool to allow access outside the class
    std::vector<Server>& getServers() {
        return servers;
    }
};

enum class SchedulingAlgorithm {
    ROUND_ROBIN,
    WEIGHTED_ROUND_ROBIN
};

void healthCheck(ServerPool& pool) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        // Randomly mark servers healthy/unhealthy for simulation
        pool.markServerHealth("server1", rand() % 2);
        pool.markServerHealth("server2", rand() % 2);
        pool.markServerHealth("server3", rand() % 2);
    }
}

class LoadBalancer {
private:
    ServerPool serverPool;
    SchedulingAlgorithm currentAlgorithm = SchedulingAlgorithm::ROUND_ROBIN;
    std::atomic<size_t> currentIndex;
    int currentWeightSum = 0;

public:
    LoadBalancer() : currentIndex(0) {}

    void addServer(const std::string& serverId, int weight = 1) {
        serverPool.addServer(serverId, weight);
    }

    void removeServer(const std::string& serverId) {
        serverPool.removeServer(serverId);
    }

    void setSchedulingAlgorithm(SchedulingAlgorithm algorithm) {
        currentAlgorithm = algorithm;
    }

    std::string getNextServer() {
        auto healthyServers = serverPool.getHealthyServers();
        if (healthyServers.empty()) {
            throw std::runtime_error("No healthy servers available");
        }

        switch (currentAlgorithm) {
            case SchedulingAlgorithm::ROUND_ROBIN: {
                size_t index = currentIndex++ % healthyServers.size();
                return healthyServers[index].id;
            }
            case SchedulingAlgorithm::WEIGHTED_ROUND_ROBIN: {
                currentWeightSum = 0;
                for (const auto& server : healthyServers) {
                    currentWeightSum += server.weight;
                }
                int randomValue = std::rand() % currentWeightSum;

                for (const auto& server : healthyServers) {
                    randomValue -= server.weight;
                    if (randomValue <= 0) {
                        return server.id;
                    }
                }

                // If the loop ends, it means the last server should be chosen
                return healthyServers.back().id;
            }
        }
    }

    // Getter for the server pool to access in health check function
    ServerPool& getServerPool() {
        return serverPool;
    }
};

void simulateTraffic(LoadBalancer& lb) {
    for (int i = 0; i < 20; ++i) { // Simulate 20 requests
        try {
            std::string server = lb.getNextServer();
            std::cout << "Request directed to: " << server << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate time delay
    }
}

int main() {
    LoadBalancer lb;

    // Add servers with weights
    lb.addServer("server1", 2); // Weight 2 for server1
    lb.addServer("server2", 1); // Weight 1 for server2
    lb.addServer("server3", 3); // Weight 3 for server3

    // Start health check routine
    std::thread healthCheckThread(healthCheck, std::ref(lb.getServerPool()));

    // Simulate traffic with Round Robin
    simulateTraffic(lb);

    // Switch to Weighted Round Robin
    lb.setSchedulingAlgorithm(SchedulingAlgorithm::WEIGHTED_ROUND_ROBIN);
    std::cout << "\nSwitched to Weighted Round Robin:\n";

    // Simulate traffic with Weighted Round Robin
    simulateTraffic(lb);

    // Simulate dynamic server pool changes
    std::this_thread::sleep_for(std::chrono::seconds(5));
    lb.removeServer("server1"); // Remove a server
    lb.addServer("server4", 1); // Add a new server with weight 1

    // Continue simulating traffic with Weighted Round Robin
    simulateTraffic(lb);

    // Clean up
    healthCheckThread.join();
    return 0;
}