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

// Function to serialize the family history tree
std::string serialize(Patient* root) {
    if (!root) return "#"; // Marker for null node

    std::stringstream ss;
    ss << root->name << "," << root->parents.size(); // Serialize name and number of parents
    for (Patient* parent : root->parents) {
        ss << "," << serialize(parent); // Recursively serialize each parent
    }
    return ss.str();
}

// Function to deserialize a string back into the family history tree
Patient* deserialize(std::stringstream& ss) {
    std::string name;
    std::getline(ss, name, ','); // Read the patient name
    if (name == "#") return nullptr; // Null check

    std::string numParentsStr;
    std::getline(ss, numParentsStr, ','); // Read number of parents
    int numParents = std::stoi(numParentsStr);

    Patient* patient = new Patient(name);

    for (int i = 0; i < numParents; ++i) {
        Patient* parent = deserialize(ss);
        if (parent) {
            patient->parents.push_back(parent); // Add the parent to this patient
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