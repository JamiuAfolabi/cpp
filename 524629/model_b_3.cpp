#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <fstream>

// Logging class to manage writing to the debug file
class Logger {
private:
    std::ofstream logFile;

public:
    Logger(const std::string& fileName) {
        logFile.open(fileName);
        if (!logFile.is_open()) {
            std::cerr << "Error: Cannot open debug file " << fileName << std::endl;
        }
    }

    // Redirect std::cout to the log file
    void redirectCout() {
        std::streambuf* origCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(logFile.rdbuf());
    }

    // Restore std::cout to the original output
    void restoreCout() {
        std::cout.rdbuf(std::cout.tie());
    }

    // Write a message to the log file
    void logMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
};

// Unique identifier for each patient to detect cycles
class Patient {
public:
    std::string name;
    std::string uniqueId;  // Added unique identifier
    std::vector<std::shared_ptr<Patient>> parents;
    
    Patient(const std::string& name, const std::string& id) 
        : name(name), uniqueId(id) {
        std::cout << "DEBUG: Created patient " << name << " with ID " << id << std::endl;
    }
};

// Helper class to track visited nodes during serialization
class CycleTracker {
private:
    std::unordered_set<std::string> visitedIds;
    
public:
    bool hasVisited(const std::string& id) {
        return visitedIds.find(id) != visitedIds.end();
    }
    
    void markVisited(const std::string& id) {
        visitedIds.insert(id);
    }
    
    void unmark(const std::string& id) {
        visitedIds.erase(id);
    }
};

// Enhanced serialization with cycle detection
std::string serialize(std::shared_ptr<Patient> root, CycleTracker& tracker) {
    if (!root) {
        std::cout << "DEBUG: Encountered null node during serialization" << std::endl;
        return "#";
    }
    
    // Check for cycles
    if (tracker.hasVisited(root->uniqueId)) {
        std::cout << "DEBUG: Cycle detected at patient " << root->name 
                  << " (ID: " << root->uniqueId << ")" << std::endl;
        return "CYCLE:" + root->uniqueId;  // Special marker for cycles
    }
    
    tracker.markVisited(root->uniqueId);
    std::cout << "DEBUG: Serializing patient " << root->name 
              << " (ID: " << root->uniqueId << ")" << std::endl;
    
    std::stringstream ss;
    ss << root->name << "," << root->uniqueId << "," << root->parents.size();
    
    for (const auto& parent : root->parents) {
        ss << "," << serialize(parent, tracker);
    }
    
    tracker.unmark(root->uniqueId);  // Unmark for backtracking
    return ss.str();
}

// Enhanced deserialization with cycle handling
std::shared_ptr<Patient> deserialize(std::stringstream& ss, 
                                   std::unordered_map<std::string, std::shared_ptr<Patient>>& idMap) {
    std::string name;
    std::getline(ss, name, ',');
    
    if (name == "#") {
        std::cout << "DEBUG: Deserializing null node" << std::endl;
        return nullptr;
    }
    
    // Handle cycles
    if (name.substr(0, 6) == "CYCLE:") {
        std::string cycleId = name.substr(6);
        std::cout << "DEBUG: Encountered cycle reference to ID: " << cycleId << std::endl;
        auto it = idMap.find(cycleId);
        if (it != idMap.end()) {
            return it->second;
        }
        throw std::runtime_error("Invalid cycle reference");
    }
    
    std::string id;
    std::getline(ss, id, ',');
    
    std::string numParentsStr;
    std::getline(ss, numParentsStr, ',');
    int numParents = std::stoi(numParentsStr);
    
    std::cout << "DEBUG: Deserializing patient " << name 
              << " (ID: " << id << ") with " << numParents << " parents" << std::endl;
    
    auto patient = std::make_shared<Patient>(name, id);
    idMap[id] = patient;  // Store for cycle resolution
    
    for (int i = 0; i < numParents; ++i) {
        auto parent = deserialize(ss, idMap);
        if (parent) {
            patient->parents.push_back(parent);
            std::cout << "DEBUG: Added parent relationship: " 
                      << parent->name << " -> " << patient->name << std::endl;
        }
    }
    
    return patient;
}

// Wrapper function for deserialization from string
std::shared_ptr<Patient> deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::unordered_map<std::string, std::shared_ptr<Patient>> idMap;
    return deserialize(ss, idMap);
}

// Forward declaration of the actual printing function
void printFamilyTreeHelper(std::shared_ptr<Patient> root, int depth, 
                         std::unordered_set<std::string>& visited);

// Wrapper function that creates the visited set
void printFamilyTree(std::shared_ptr<Patient> root, int depth = 0) {
    std::unordered_set<std::string> visited;
    printFamilyTreeHelper(root, depth, visited);
}

// Actual implementation of tree printing with cycle detection
void printFamilyTreeHelper(std::shared_ptr<Patient> root, int depth, 
                         std::unordered_set<std::string>& visited) {
    if (!root) return;
    
    std::string indent(depth * 2, ' ');
    std::cout << indent << "Patient: " << root->name << " (ID: " << root->uniqueId << ")";
    
    if (visited.find(root->uniqueId) != visited.end()) {
        std::cout << " [CYCLE DETECTED]" << std::endl;
        return;
    }
    
    std::cout << std::endl;
    visited.insert(root->uniqueId);
    
    for (const auto& parent : root->parents) {
        printFamilyTreeHelper(parent, depth + 1, visited);
    }
    
    visited.erase(root->uniqueId);
}

int main() {
    
    Logger logger("debug.log");

    logger.redirectCout();

    // Create a family tree with a cycle
    auto child = std::make_shared<Patient>("Child", "P1");
    auto parent1 = std::make_shared<Patient>("Parent1", "P2");
    auto parent2 = std::make_shared<Patient>("Parent2", "P3");
    auto grandparent = std::make_shared<Patient>("Grandparent", "P4");
    
    child->parents.push_back(parent1);
    child->parents.push_back(parent2);
    parent1->parents.push_back(grandparent);
    
    // Create a cycle: grandparent is also child's child (impossible in reality, but good for testing)
    grandparent->parents.push_back(child);
    
    // Serialize with cycle detection
    CycleTracker tracker;
    std::string serializedData = serialize(child, tracker);
    std::cout << "\nSerialized Data: " << serializedData << std::endl;
    
    // Deserialize
    auto deserializedChild = deserialize(serializedData);
    
    // Print the tree with cycle detection
    std::cout << "\nDeserialized Family Tree:" << std::endl;
    printFamilyTree(deserializedChild);

    logger.restoreCout();
    
    return 0;
}