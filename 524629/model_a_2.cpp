#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class Patient {
public:
    std::string name;
    std::vector<Patient*> parents; // Store pointers to parent nodes

    Patient(const std::string& name) : name(name) {}
};

// Serialize helper
std::string serialize(Patient* root, std::unordered_set<Patient*>& seen) {
    if (!root) return "#"; // Marker for null node

    if (seen.find(root) != seen.end()) {
        std::cout << "Cycle detected: " << root->name << std::endl;
        return root->name + "*"; // Mark this patient as part of a cycle
    }

    seen.insert(root); // Mark this patient as seen
    std::stringstream ss;
    ss << root->name << "," << root->parents.size(); // Serialize name and number of parents
    for (Patient* parent : root->parents) {
        ss << "," << serialize(parent, seen); // Recursively serialize each parent
    }
    return ss.str();
}

// Serialize the family history tree
std::string serialize(Patient* root) {
    std::unordered_set<Patient*> seen; // to track seen patients
    return serialize(root, seen);
}

// Deserialize helper
Patient* deserialize(std::stringstream& ss, std::unordered_map<std::string, Patient*>& created) {
    std::string name;
    std::getline(ss, name, ','); // Read the patient name
    if (name == "#") return nullptr; // Null check

    // Check for cycle marker
    bool isCycle = false;
    if (name.back() == '*') {
        isCycle = true;
        name.pop_back(); // Remove cycle marker
    }

    // Check if this patient was already created
    if (created.find(name) != created.end()) {
        return created[name]; // Return existing patient object
    }

    // Create a new patient
    Patient* patient = new Patient(name);
    created[name] = patient; // Map this patient name to the patient object

    std::string numParentsStr;
    std::getline(ss, numParentsStr, ','); // Read number of parents
    int numParents = std::stoi(numParentsStr);

    for (int i = 0; i < numParents; ++i) {
        Patient* parent = deserialize(ss, created);
        if (parent) {
            patient->parents.push_back(parent); // Add the parent to this patient
        }
    }
    
    return patient;
}

// Deserialize the serialized string
Patient* deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::unordered_map<std::string, Patient*> created; // to track created patients
    return deserialize(ss, created);
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
    // Create a sample family history with a cycle
    Patient* child = new Patient("Child");
    Patient* parent1 = new Patient("Parent1");
    Patient* parent2 = new Patient("Parent2");
    
    child->parents.push_back(parent1);
    child->parents.push_back(parent2);

    Patient* grandparent1 = new Patient("Grandparent1");
    Patient* grandparent2 = new Patient("Grandparent2");
    
    parent1->parents.push_back(grandparent1);
    parent1->parents.push_back(grandparent2);

    // Introduce a cycle
    grandparent2->parents.push_back(parent1); // Cycle back to Parent1

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