#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <random>  // Added for random number generation

struct Server {
    std::string id;
    bool isHealthy;
    int weight;  // Weight for the server in Weighted Round Robin
    int retryCount = 0; // Added to keep track of retry attempts
    int maxRetries = 3; // Number of max retry attempts

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
        std::cout << "Added server: " << serverId << " (weight=" << weight << ")" << std::endl;
    }

    void removeServer(const std::string& serverId) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = std::remove_if(servers.begin(), servers.end(), [&](const Server& server) {
            return server.id == serverId;
        });
        servers.erase(it, servers.end());
        std::cout << "Removed server: " << serverId << std::endl;
    }

    void markServerHealth(const std::string& serverId, bool healthStatus) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& server : servers) {
            if (server.id == serverId) {
                if (!healthStatus) {
                    server.retryCount++;
                    std::cout << "Server " << server.id << " marked unhealthy. Retry count: " << server.retryCount << std::endl;
                    if (server.retryCount <= server.maxRetries) {
                        std::cout << "Server " << server.id << " will be retried. Waiting for recovery..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate recovery time
                        server.isHealthy = true;
                        server.retryCount = 0;
                        std::cout << "Server " << server.id << " recovered." << std::endl;
                        return;
                    }
                }
                server.isHealthy = healthStatus;
                server.retryCount = 0;
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

    // Getter for the server pool to allow access outside the class
    std::vector<Server>& getServers() {
        return servers;
    }
};

void healthCheck(ServerPool& pool) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        // Randomly mark servers healthy/unhealthy for simulation
        auto servers = pool.getServers();
        for (auto& server : servers) {
            int randomHealth = rand() % 2;
            if (randomHealth == 0) {
                pool.markServerHealth(server.id, false);
            }
        }
    }
}

class LoadBalancer {
private:
    ServerPool serverPool;
    std::atomic<size_t> currentIndex;
    bool useWeightedRoundRobin;

public:
    LoadBalancer(bool weighted = false) : currentIndex(0), useWeightedRoundRobin(weighted) {}

    void addServer(const std::string& serverId, int weight = 1) {
        serverPool.addServer(serverId, weight);
    }

    void removeServer(const std::string& serverId) {
        serverPool.removeServer(serverId);
    }

    std::string getNextServer() {
        auto healthyServers = serverPool.getHealthyServers();
        if (healthyServers.empty()) {
            throw std::runtime_error("No healthy servers available");
        }

        while (true) {
            if (useWeightedRoundRobin) {
                std::string server = getNextWeightedServer(healthyServers);
                if (!server.empty()) {
                    return server;
                }
            } else {
                std::string server = getNextRoundRobinServer(healthyServers);
                if (!server.empty()) {
                    return server;
                }
            }
        }
    }

    // Getter for the server pool to access in health check function
    ServerPool& getServerPool() {
        return serverPool;
    }

private:
    // Standard Round Robin algorithm
    std::string getNextRoundRobinServer(const std::vector<Server>& healthyServers) {
        size_t index = currentIndex++ % healthyServers.size();
        return healthyServers[index].id;
    }

    // Weighted Round Robin algorithm
    std::string getNextWeightedServer(const std::vector<Server>& healthyServers) {
        static size_t currentWeightIndex = 0;
        int totalWeight = 0;
        for (const auto& server : healthyServers) {
            totalWeight += server.weight;
        }

        currentWeightIndex = (currentWeightIndex + 1) % totalWeight;

        int weightSum = 0;
        for (const auto& server : healthyServers) {
            weightSum += server.weight;
            if (weightSum > currentWeightIndex) {
                return server.id;
            }
        }

        return healthyServers.back().id; // Fallback in case of any issue
    }
};

void simulateTraffic(LoadBalancer& lb) {
    while (true) {
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
    int choice;
    std::cout << "Choose Load Balancing Algorithm:\n1. Standard Round Robin\n2. Weighted Round Robin\n";
    std::cin >> choice;

    LoadBalancer lb(choice == 2);  // Choose the algorithm based on user input

    // Add servers with weights
    lb.addServer("server1", 1);  // Weight 1
    lb.addServer("server2", 2);  // Weight 2
    lb.addServer("server3", 3);  // Weight 3

    // Start health check routine
    std::thread healthCheckThread(healthCheck, std::ref(lb.getServerPool()));  // Pass ServerPool by reference

    // Simulate traffic
    simulateTraffic(lb);

    // Simulate dynamic server pool changes (adding/removing servers)
    std::this_thread::sleep_for(std::chrono::seconds(15));
    lb.removeServer("server1"); // Remove a server
    lb.addServer("server4", 4); // Add a new server

    // Continue simulating traffic
    simulateTraffic(lb);

    // Clean up
    healthCheckThread.join();
    return 0;
}
