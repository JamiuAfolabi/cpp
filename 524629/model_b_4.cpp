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
#include <ctime>
#include <iomanip>


// Global ofstream for logging
std::ofstream debugLog;

// Logging function
void logDebug(const std::string& message, const std::string& patientId = "") {
    if (debugLog.is_open()) {
        // Get current timestamp
        time_t now = time(0);
        tm* ts = localtime(&now);
        
        // Format timestamp
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
        
        debugLog << buf << " " << patientId << ": " << message << std::endl;
    }
}

// Unique identifier for each patient to detect cycles
class Patient {
public:
    std::string name;
    std::string uniqueId;  // Added unique identifier
    std::vector<std::shared_ptr<Patient>> parents;
    
    Patient(const std::string& name, const std::string& id) 
        : name(name), uniqueId(id) {
        logDebug("Created patient " + name + " with ID " + id, id);
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
        logDebug("DEBUG: Encountered null node during serialization");
        return "#";
    }
    
    // Check for cycles
    if (tracker.hasVisited(root->uniqueId)) {
        logDebug("DEBUG: Cycle detected at patient " + root->name 
                  + " (ID: " + root->uniqueId + ")");
        return "CYCLE:" + root->uniqueId;  // Special marker for cycles
    }
    
    tracker.markVisited(root->uniqueId);
    logDebug("DEBUG: Serializing patient " + root->name 
              + " (ID: " + root->uniqueId + ")");
    
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
        logDebug("DEBUG: Deserializing null node");
        return nullptr;
    }
    
    // Handle cycles
    if (name.substr(0, 6) == "CYCLE:") {
        std::string cycleId = name.substr(6);
        logDebug("DEBUG: Encountered cycle reference to ID: " + cycleId);
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
    
    logDebug("DEBUG: Deserializing patient " + name 
              + " (ID: " + id + ") with " + std::to_string(numParents) + " parents");
    
    auto patient = std::make_shared<Patient>(name, id);
    idMap[id] = patient;  // Store for cycle resolution
    
    for (int i = 0; i < numParents; ++i) {
        auto parent = deserialize(ss, idMap);
        if (parent) {
            patient->parents.push_back(parent);
            logDebug("DEBUG: Added parent relationship: " 
                      + parent->name + " -> " + patient->name);
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
    // Open the debug log file
    debugLog.open("debug_log.txt");
    if (!debugLog.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

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

    // Close the debug log file
    debugLog.close();
    
    return 0;
}