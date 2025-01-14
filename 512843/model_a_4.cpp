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
    int weight;  // Weight for the server in Weighted Round Robin

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
        std::cout << "Added server: " << serverId << " with weight: " << weight << std::endl;
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
                server.isHealthy = healthStatus;
                std::cout << "Server " << serverId << " marked as " << (healthStatus ? "healthy" : "unhealthy") << std::endl;
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
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // Check the health of each server
        for (const auto& server : pool.getServers()) {
            // Simulating random health status; replace with actual health check logic
            bool isHealthy = rand() % 2;
            if (!isHealthy) {
                std::cout << "Health check failed for server: " << server.id << std::endl;
                
                // Retry mechanism
                int retries = 3;
                while (retries > 0) {
                    std::cout << "Retrying health check for server: " << server.id << " (" << retries << " retries left)" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    isHealthy = rand() % 2; // Re-evaluate health status
                    if (isHealthy) {
                        pool.markServerHealth(server.id, true);
                        std::cout << "Server " << server.id << " recovered and marked healthy." << std::endl;
                        break;
                    }
                    retries--;
                }

                // If still unhealthy, mark server as unhealthy
                if (retries == 0) {
                    pool.markServerHealth(server.id, false);
                    std::cout << "Server " << server.id << " is permanently marked as unhealthy. Removing server..." << std::endl;
                    pool.removeServer(server.id);
                }
            }
        }
    }
}

class LoadBalancer {
private:
    ServerPool serverPool;
    std::atomic<size_t> currentIndex;
    bool useWeightedRoundRobin;  // Flag to select the algorithm

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

        if (useWeightedRoundRobin) {
            return getNextWeightedServer(healthyServers);
        } else {
            return getNextRoundRobinServer(healthyServers);
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
    for (int i = 0; i < 10; ++i) { // Simulate 10 requests
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
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    lb.removeServer("server1"); // Remove a server
    lb.addServer("server4", 4); // Add a new server

    // Continue simulating traffic
    simulateTraffic(lb);

    // Clean up
    healthCheckThread.join();
    return 0;
}