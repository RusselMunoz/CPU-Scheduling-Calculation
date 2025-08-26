#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// Helper function: return a sorted vector by value
vector<pair<string, int>> getSorted(const unordered_map<string,int>& processes) {
    vector<pair<string, int>> vec(processes.begin(), processes.end());
    sort(vec.begin(), vec.end(), [](auto &a, auto &b) {
        return a.second < b.second;
    });
    return vec;
}

// Calculate waiting time from sorted vector
double waitingTime(const vector<pair<string, int>>& vec){
    double totalWait = 0.0;
    double cumulative = 0.0;
    // waiting time for each process (except the first)
    for (int i = 0; i < vec.size(); i++) {
        totalWait += cumulative;   // add the time this process had to wait
        cumulative += vec[i].second; // update cumulative burst time
    }
    // exclude the dummy "The Fuck" if you don't want it in avg
    return totalWait / (vec.size() - 1);
}

// Calculate turn around time from sorted vector
double turnaround(const vector<pair<string, int>>& vec) {
    double totalTAT = 0.0;
    double cumulative = 0.0;
    
    for (int i = 0; i < vec.size(); i++) {
        cumulative += vec[i].second; // completion time
        totalTAT += cumulative;      // turnaround time = completion time (assuming arrival time = 0)
    }
    // exclude the dummy "The Fuck" in avg
    return totalTAT / (vec.size() - 1);
}

// Prints processes and accumulates step by step
void proc(unordered_map<string, int>& processes){
    int sum = 0;
    // add test data
    processes["The Fuck"] = 0;
    processes["Process 1"] = 5;
    processes["Process 2"] = 3;
    processes["Process 3"] = 8;
    processes["Process 4"] = 2;
    
    // get sorted processes
    auto vec = getSorted(processes);
    
    // Print sorted
    for (auto& p : vec) {
        cout << p.first << " -> " << p.second << endl;
    }
    
    // Step by step accumulation (completion times)
    cout << "\nCompletion times: ";
    int cumulative = 0;
    for (int i = 0; i < vec.size(); i++) {
        cumulative += vec[i].second;
        cout << cumulative << " ";
    }
    cout << endl;
    
    // Print waiting time & turnaround
    cout << "Average waiting time: " << waitingTime(vec) << "m/s" << endl;
    cout << "Average turnaround time: " << turnaround(vec) << "m/s" << endl;
}

int main() {
    unordered_map<string, int> processes;
    proc(processes);  // this will also print waiting time
    return 0;
}