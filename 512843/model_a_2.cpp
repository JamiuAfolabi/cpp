#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <stdexcept>

struct Server {
    std::string id;
    bool isHealthy;
    int weight; // Weight for the Weighted Round Robin

    Server(const std::string& id, int weight = 1) : id(id), isHealthy(true), weight(weight) {}
};

// ServerPool manages a list of servers.
class ServerPool {
private:
    std::vector<Server> servers;
    std::mutex mtx; // To protect shared resources

public:
    void addServer(const std::string& serverId, int weight = 1) {
        std::lock_guard<std::mutex> lock(mtx);
        servers.emplace_back(serverId, weight);
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

    std::vector<Server>& getServers() {
        return servers;
    }
};

void healthCheck(ServerPool& pool) {
    while (true) {
        // Simulating health checks with dummy logic
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        // Randomly mark servers healthy/unhealthy for simulation
        pool.markServerHealth("server1", rand() % 2);
        pool.markServerHealth("server2", rand() % 2);
        pool.markServerHealth("server3", rand() % 2);
        pool.markServerHealth("server4", rand() % 2); // If added
    }
}

class LoadBalancer {
private:
    ServerPool serverPool;
    std::atomic<size_t> currentIndex;
    int currentWeight;
    int maxWeight;
    int gcd;

public:
    enum class Algorithm { ROUND_ROBIN, WEIGHTED_ROUND_ROBIN };

    LoadBalancer() : currentIndex(0), currentWeight(0), maxWeight(0), gcd(0) {}

    void addServer(const std::string& serverId, int weight = 1) {
        serverPool.addServer(serverId, weight);
        maxWeight = std::max(maxWeight, weight); // Update max weight
    }

    void removeServer(const std::string& serverId) {
        serverPool.removeServer(serverId);
        // Recompute max weight if necessary
        maxWeight = 0;
        for (const auto& server : serverPool.getServers()) {
            if (server.isHealthy) {
                maxWeight = std::max(maxWeight, server.weight);
            }
        }
    }

    std::string getNextServer(Algorithm algo) {
        switch (algo) {
            case Algorithm::ROUND_ROBIN:
                return getNextServerRoundRobin();
            case Algorithm::WEIGHTED_ROUND_ROBIN:
                return getNextServerWeightedRoundRobin();
            default:
                throw std::runtime_error("Unknown Load Balancing Algorithm");
        }
    }

private:
    std::string getNextServerRoundRobin() {
        auto healthyServers = serverPool.getHealthyServers();
        if (healthyServers.empty()) {
            throw std::runtime_error("No healthy servers available");
        } 

        // Round Robin
        size_t index = currentIndex++ % healthyServers.size();
        return healthyServers[index].id;
    }

    std::string getNextServerWeightedRoundRobin() {
        auto healthyServers = serverPool.getHealthyServers();
        if (healthyServers.empty()) {
            throw std::runtime_error("No healthy servers available");
        }

        while (true) {
            for (const auto& server : healthyServers) {
                if (server.isHealthy) {
                    currentWeight += server.weight;

                    if (currentWeight >= maxWeight) {
                        currentWeight -= maxWeight; // Reset on reaching max weight
                        return server.id; // Return the selected server
                    }
                }
            }
        }
    }
};

void simulateTraffic(LoadBalancer& lb, LoadBalancer::Algorithm algo) {
    for (int i = 0; i < 10; ++i) { // Simulate 10 requests
        try {
            std::string server = lb.getNextServer(algo);
            std::cout << "Request directed to: " << server << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate time delay
    }
}

int main() {
    LoadBalancer lb;

    // Add servers with different weights
    lb.addServer("server1", 2); // Weight 2
    lb.addServer("server2", 1); // Weight 1
    lb.addServer("server3", 1); // Weight 1
    lb.addServer("server4", 3); // Weight 3

    // Start health check routine
    std::thread healthCheckThread(healthCheck, std::ref(lb.getServerPool())); // Pass ServerPool by reference

    // Simulate traffic using standard Round Robin
    std::cout << "Simulating traffic using Round Robin:" << std::endl;
    simulateTraffic(lb, LoadBalancer::Algorithm::ROUND_ROBIN);

    // Pause before switching algorithms
    std::this_thread::sleep_for(std::chrono::seconds(5)); 

    // Simulate traffic using Weighted Round Robin
    std::cout << "\nSimulating traffic using Weighted Round Robin:" << std::endl;
    simulateTraffic(lb, LoadBalancer::Algorithm::WEIGHTED_ROUND_ROBIN);

    // Clean up
    healthCheckThread.join();
    return 0;
}