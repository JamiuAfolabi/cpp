#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Serializer {
private:
    unordered_map<size_t, unsigned int> IDs;
    unsigned int nextID = 1;
    static void storeObject(vector<string>& data, void* obj, void* parent);
    static void assembleObject(const vector<string>& data, void* obj);

template <typename T>
    static inline string GetTypeName() { return typeid(T).name(); }

public:
    template <typename T>
    static void Serialize(T obj, string& output, bool verbose = false) {
        vector<string> data;
        Serializer s;
        s.storeObject(data, &obj, nullptr);
        if(verbose) {
            for (const string& entry : data) {
                cout << entry << endl;
            }
        }
        output = "[" + data.front() + ", [" + data.back() + "]\n" +
                  join(data.begin() + 2, data.end() - 1, ", ", "");
    }

    template <typename T>
    static void Deserialize(const string& input, T& obj) {
        vector<string> data;
        istringstream iss(input);
        getline(iss, data[0]); 
        getline(iss, data[data.size() - 1]);
        remove_if(data.begin(), data.end() - 1, [](const string& s) { return s.empty(); });
        
        Serializer s;
        s.assembleObject(data, &obj);
    }

    static string generateID() {
        string s(to_string(nextID));
        nextID++;
        return s;
    }
};

class Structure {
public:
    Structure() : id("") {}
    Structure(string id) : id(id) {}
    string id;
    string field;
    vector<Structure*> pointernested;
    vector<Structure&> refnested;
};

struct Object {
    int value = 42;
    string name = "My Object";
    double pi = 3.14;
    vector<int> arr1 = { 1, 2, 3 };
    vector<Object*> arr2;
};

void Serializer::storeObject(vector<string>& data, void* obj, void* parent) {
    unsigned int currentID;
    if(IDs.find((size_t)obj) != IDs.end()) {
        currentID = IDs[(size_t)obj];
        data.push_back("R:" + to_string(currentID));
        return;
    }

    string objName = Serializer::GetTypeName(*(Object*)obj);
    currentID = IDs[(size_t)obj] = nextID++;
    data.push_back("O:" + objName + ":" + Serializer::generateID());
    
    Object* o = (Object*)obj;
    data.push_back("F:value:" + to_string(o->value));
    data.push_back("F:name:" + o->name);
    data.push_back("F:pi:" + to_string(o->pi));

    size_t ptrsz = o->arr2.size();
    data.push_back("A:arr2:" + to_string(ptrsz));
    for (size_t i = 0; i < ptrsz; ++i) {
        if (o->arr2[i]) {
            storeObject(data, (void*)o->arr2[i], obj);
        }
    }

    data.push_back("-");
}

void Serializer::assembleObject(const vector<string>& data, void* obj) {
    istringstream iss(data[2]);
    Object* o = (Object*)obj;

    string token;
    while (getline(iss, token, ':')) {
        if (token == "F:value") {
            getline(iss, token);