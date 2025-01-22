#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

class Patient {
public:
    std::string name;
    std::vector<Patient*> parents; // Store pointers to parent nodes

    Patient(const std::string& name) : name(name) {}
};

std::string serialize(Patient* root, std::set<Patient*>& visited) {
    if (!root) return "#"; // Marker for null node

    if (visited.find(root) != visited.end()) {
        std::cout << "Cycle detected: Patient " << root->name << " has already been visited." << std::endl;
        return "~"; // Use '~' to indicate a cycle
    }

    visited.insert(root);

    std::stringstream ss;
    ss << root->name << "," << root->parents.size();
    for (Patient* parent : root->parents) {
        ss << "," << serialize(parent, visited);
    }
    visited.erase(root);
    return ss.str();
}

std::string serialize(Patient* root) {
    std::set<Patient*> visited;
    return serialize(root, visited);
}

Patient* deserialize(std::stringstream& ss) {
    std::string name;
    std::getline(ss, name, ',');
    if (name == "#") return nullptr;
    if (name == "~") {
        std::cout << "Cycle detected during deserialization." << std::endl;
        return nullptr;
    }

    std::string numParentsStr;
    std::getline(ss, numParentsStr, ',');
    int numParents = std::stoi(numParentsStr);

    Patient* patient = new Patient(name);

    for (int i = 0; i < numParents; ++i) {
        Patient* parent = deserialize(ss);
        if (parent) {
            patient->parents.push_back(parent);
        }
    }

    return patient;
}

// Function to deserialize from a serialized string
Patient* deserialize(const std::string& data) {
    std::stringstream ss(data);
    return deserialize(ss);
}

// Function to print the family tree for debugging
void printFamilyTree(Patient* root, int depth = 0) {
    if (!root) return;

    std::cout << std::string(depth * 2, ' ') << "Patient: " << root->name << std::endl;
    for (Patient* parent : root->parents) {
        printFamilyTree(parent, depth + 1);
    }
}

// Example usage
int main() {
    // Create a sample family history
    Patient* child = new Patient("Child");
    Patient* parent1 = new Patient("Parent1");
    Patient* parent2 = new Patient("Parent2");
    
    child->parents.push_back(parent1);
    child->parents.push_back(parent2);

    Patient* grandparent1 = new Patient("Grandparent1");
    Patient* grandparent2 = new Patient("Grandparent2");
    
    parent1->parents.push_back(grandparent1);
    parent1->parents.push_back(grandparent2);

    // Serialize
    std::string serializedData = serialize(child);
    std::cout << "Serialized Data: " << serializedData << std::endl;

    // Deserialize
    Patient* deserializedChild = deserialize(serializedData);
    std::cout << "Deserialized Family Tree:" << std::endl;
    printFamilyTree(deserializedChild);

    // Clean up (memory management would be better with smart pointers in a real application)
    delete child; 
    delete parent1; 
    delete parent2; 
    delete grandparent1; 
    delete grandparent2; 

    return 0;
}