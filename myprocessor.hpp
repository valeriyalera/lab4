#ifndef MYPROCESSOR_HPP
#define MYPROCESSOR_HPP

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

class DataStruct {
private:
    int fields[3] = {0,0,0};
    mutex m[3];
    mutex stringLock;

public:
    int read(int i) {
        if (i < 0 || i >= 3) return -1;
        lock_guard<mutex> lock(m[i]);
        return fields[i];
    }

    void write(int i, int value) {
        if (i < 0 || i >= 3) return;
        lock_guard<mutex> lock(m[i]);
        fields[i] = value;
    }

    operator string() {
        lock_guard<mutex> lock(stringLock);
        for (int i = 0; i < 3; i++) {
            m[i].lock();
        }
        
        string result = "[" + to_string(fields[0]) + ", " 
                           + to_string(fields[1]) + ", " 
                           + to_string(fields[2]) + "]";
        
        for (int i = 0; i < 3; i++) {
            m[i].unlock();
        }
        
        return result;
    }
};

void runFile(const string &filename, DataStruct &ds) {
    ifstream file(filename);
    string op;
    int field, value;

    while (file >> op) {
        if (op == "read") {
            file >> field;
            ds.read(field);
        }
        else if (op == "write") {
            file >> field >> value;
            ds.write(field, value);
        }
        else if (op == "string") {
            string result = static_cast<string>(ds); 
        }
    }
}

#endif