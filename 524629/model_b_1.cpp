
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#ifndef PATIENT_H
#define PATIENT_H

class Patient {
public:
    std::string name;
    int id;
    Patient* mom;
    Patient* dad;

    Patient(std::string name, int id) : name(name), id(id), mom(nullptr), dad(nullptr) {}

    ~Patient() {
        delete mom;
        delete dad;
    }
};

#endif

std::string serializePatient(Patient* patient) {
    if (patient == nullptr) {
        return "null";
    }

    std::stringstream ss;
    ss << patient->id << "," << patient->name << ",";
    // Serialize mother and father
    ss << serializePatient(patient->mom) << "," << serializePatient(patient->dad);
    return ss.str();
}

Patient* deserializePatient(std::vector<Patient*>& patients, std::stringstream& ss) {
    std::string idStr, nameStr;
    std::getline(ss, idStr, ',');
    std::getline(ss, nameStr, ',');
    if (idStr == "null") {
        return nullptr;
    }

    int id = std::stoi(idStr);
    for (Patient* patient : patients) {
        if (patient->id == id) {
            // Found the patient, verify they don't have parents already
            if (patient->mom != nullptr || patient->dad != nullptr) {
                throw std::runtime_error(f"Patient {nameStr} with ID {id} already has parents!");
            }
            return patient;
        }
    }

    // Patient not found, create a new one
    Patient* newPatient = new Patient(nameStr, id);
    patients.push_back(newPatient);
    // Deserialize and assign parents
    newPatient->mom = deserializePatient(patients, ss);
    newPatient->dad = deserializePatient(patients, ss);

    return newPatient;
}

void printPatientTree(Patient* patient) {
    if (patient == nullptr) return;

    std::cout << patient->name << " (ID: " << patient->id << ")" << std::endl;
    printPatientTree(patient->mom);
    printPatientTree(patient->dad);
}

int main() {
    // Create a simple sample tree
    Patient* root = new Patient("John Doe", 1);
    root->mom = new Patient("Jane Doe", 2);
    root->dad = new Patient("Jim Doe", 3);
    root->mom->mom = new Patient("Mary Smith", 4);

    std::cout << "Original Patient Tree:" << std::endl;
    printPatientTree(root);

    // Serialization
    std::string serializedData = "";
    std::stringstream ss;
    ss << serializePatient(root);
    serializedData = ss.str();

    std::cout << "\nSerialized Data: " << serializedData << std::endl;

    // Deserialization
    std::vector<Patient*> patientsAfterDeserialization;
    Patient* deserializedRoot = nullptr;
    std::stringstream deserializeSs(serializedData);
    deserializedRoot = deserializePatient(patientsAfterDeserialization, deserializeSs);

    std::cout << "\nDeserialized Patient Tree:" << std::endl;
    printPatientTree(deserializedRoot);

    // Verify Relationships
    std::cout << "\nVerifying Relationships:" << std::endl;
    Patient* testPatient = deserializedRoot->mom->mom;