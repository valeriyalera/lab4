// Potiekhina Valeriia, K-27, var5
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include "myprocessor.hpp"

using namespace std;

void generateFile(const string& filename,
                  const vector<double>& probs,
                  int lines)
{
    ofstream out(filename);
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<int> dist(probs.begin(), probs.end());

    for (int i = 0; i < lines; i++)
    {
        int op = dist(gen);
        switch (op)
        {
            case 0: out << "read 0\n"; break;
            case 1: out << "write 0 1\n"; break;
            case 2: out << "read 1\n"; break;
            case 3: out << "write 1 1\n"; break;
            case 4: out << "read 2\n"; break;
            case 5: out << "write 2 1\n"; break;
            case 6: out << "string\n"; break;
        }
    }
    out.close();
    cout << "File " << filename << " created!\n";
}

void measure(const string& filename, int threadsCount)
{
    DataStruct ds;

    auto start = chrono::high_resolution_clock::now();

    vector<thread> threads;
    for (int i = 0; i < threadsCount; i++)
        threads.emplace_back(runFile, filename, ref(ds));

    for (auto& t : threads) t.join();

    auto end = chrono::high_resolution_clock::now();
    auto time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    auto time_us = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Threads: " << threadsCount << " | File: " << filename 
         << " | Time: " << time_ms << " ms (" << time_us << " μs)\n";
}

int main() {
    int LINES = 100000; 
    vector<double> probsA = {9,1,40,10,10,10,20};
    vector<double> probsB = {1,1,1,1,1,1,1};
    vector<double> probsC = {50,2,2,1,2,40,3};

    generateFile("caseA.txt", probsA, LINES);
    generateFile("caseB.txt", probsB, LINES);
    generateFile("caseC.txt", probsC, LINES);

    vector<string> files = {"caseA.txt","caseB.txt","caseC.txt"};

    cout << "\n=== PERFORMANCE RESULTS ===\n";
    for (auto &f : files) {
        measure(f, 1);
        measure(f, 2);
        measure(f, 3);
        cout << "---\n";
    }

    return 0;
}