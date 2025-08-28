#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

// Return a sorted vector of (processId, burstTime)
vector<pair<int,int>> getSorted(const vector<int>& processes) {
    vector<pair<int,int>> vec;
    for (int i = 0; i < processes.size(); i++) {
        vec.push_back({i+1, processes[i]}); // process IDs start from 1
    }
    sort(vec.begin(), vec.end(), [](auto &a, auto &b) {
        return a.second < b.second;
    });
    return vec;
}

// Calculate waiting times for each process
vector<int> getWaitingTimes(const vector<pair<int,int>>& vec) {
    vector<int> waiting(vec.size(), 0);
    int cumulative = 0;
    for (int i = 0; i < vec.size(); i++) {
        waiting[i] = cumulative;
        cumulative += vec[i].second;
    }
    return waiting;
}

// Calculate turnaround times for each process
vector<int> getTurnaroundTimes(const vector<pair<int,int>>& vec) {
    vector<int> tat(vec.size(), 0);
    int cumulative = 0;
    for (int i = 0; i < vec.size(); i++) {
        cumulative += vec[i].second;
        tat[i] = cumulative;
    }
    return tat;
}

// Input processes and print scheduling results
void proc(vector<int>& processes) {
    int burstTime;
    cout << "Enter burst times (0 to stop):" << endl;
    while (cin >> burstTime && burstTime != 0) {
        processes.push_back(burstTime);
        cout << "Process " << processes.size() << ": " << burstTime << endl;
    }

    // Sort by burst time
    auto vec = getSorted(processes);

    // Compute waiting times and turnaround times
    auto waiting = getWaitingTimes(vec);
    auto tat = getTurnaroundTimes(vec);

    // Print sorted processes
    cout << "\nProcesses sorted by burst time:\n";
    for (int i = 0; i < vec.size(); i++) {
        cout << "Process " << vec[i].first 
             << " -> Burst: " << vec[i].second
             << " | Waiting: " << waiting[i]
             << " | Turnaround: " << tat[i] << endl;
    }

    // Print Gantt chart
    cout << "\nGantt Chart:\n";
    cout << "0";  // always start at 0
    int cumulative = 0;
    for (int i = 0; i < vec.size(); i++) {
        cumulative += vec[i].second;
        cout << " " << cumulative << " ";
    }
    cout << endl;

    // Averages
    double avgWait = 0, avgTAT = 0;
    for (int i = 0; i < vec.size(); i++) {
        avgWait += waiting[i];
        avgTAT += tat[i];
    }
    cout << "\nAverage waiting time: " << avgWait / vec.size() << endl;
    cout << "Average turnaround time: " << avgTAT / vec.size() << endl;
}

int main() {
    vector<int> processes;
    auto start = chrono::high_resolution_clock::now();

    proc(processes);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    return 0;
}
