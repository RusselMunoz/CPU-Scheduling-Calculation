#include <iostream>     // for input/output
#include <vector>       // for using std::vector
#include <algorithm>    // for std::sort
#include <chrono>       // for measuring execution time
#include <unordered_set> // for checking duplicate arrival times
using namespace std;

// ==============================
// Function: getSorted
// Purpose: Take vectors of burst times and arrival times, and return a vector of (processId, burstTime, arrivalTime) tuples
// Sorted in ascending order by burst time, arrival time, and process ID
// ==============================
vector<tuple<int, int, int>> getSorted(const vector<int>& burstTimes, const vector<int>& arrivalTimes) {
    vector<tuple<int, int, int>> vec;

    // Assign IDs to processes (starting from 1) and pair them with their burst and arrival times
    for (int i = 0; i < burstTimes.size(); i++) {
        vec.push_back({i + 1, burstTimes[i], arrivalTimes[i]});
    }

    // Sort the vector of tuples using a lambda comparator
    // Comparator first compares burst time, then arrival time, and finally process ID
    sort(vec.begin(), vec.end(), [](auto &a, auto &b) {
        if (get<1>(a) != get<1>(b)) // Compare burst times
            return get<1>(a) < get<1>(b);
        if (get<2>(a) != get<2>(b)) // Compare arrival times
            return get<2>(a) < get<2>(b);
        return get<0>(a) < get<0>(b); // Compare process IDs
    });

    return vec; // return sorted vector
}

// ==============================
// Function: getWaitingTimes
// Purpose: Calculate waiting times for each process in sorted order
// Waiting time = sum of burst times of all previous processes - arrival time
// ==============================
vector<int> getWaitingTimes(const vector<tuple<int, int, int>>& vec) {
    vector<int> waiting(vec.size(), 0); // initialize all waiting times to 0
    int cumulative = 0; // keeps track of elapsed CPU time

    for (int i = 0; i < vec.size(); i++) {
        int arrivalTime = get<2>(vec[i]);
        waiting[i] = max(0, cumulative - arrivalTime); // waiting time cannot be negative
        cumulative = max(cumulative, arrivalTime) + get<1>(vec[i]); // update cumulative time
    }
    return waiting;
}

// ==============================
// Function: getTurnaroundTimes
// Purpose: Calculate turnaround times for each process
// Turnaround time = waiting time + burst time
// ==============================
vector<int> getTurnaroundTimes(const vector<tuple<int, int, int>>& vec, const vector<int>& waiting) {
    vector<int> tat(vec.size(), 0);

    for (int i = 0; i < vec.size(); i++) {
        tat[i] = waiting[i] + get<1>(vec[i]); // turnaround = waiting time + burst time
    }
    return tat;
}

// ==============================
// Function: proc
// Purpose: Handles user input, calculates scheduling results, and prints them
// ==============================
void proc(vector<int>& burstTimes, vector<int>& arrivalTimes) {
    int burstTime, arrivalTime;
    unordered_set<int> arrivalSet; // to track duplicate arrival times

    // Step 1: Input burst times and arrival times from user until they enter 0
    cout << "Enter burst times and arrival times (0 0 to stop):" << endl;
    while (cin >> burstTime >> arrivalTime && (burstTime != 0 || arrivalTime != 0)) {
        if (arrivalSet.find(arrivalTime) != arrivalSet.end()) {
            cout << "Error: Duplicate arrival time detected. Please enter a unique arrival time." << endl;
            continue;
        }
        burstTimes.push_back(burstTime); // add burst time to list
        arrivalTimes.push_back(arrivalTime); // add arrival time to list
        arrivalSet.insert(arrivalTime); // mark arrival time as used
        cout << "Process " << burstTimes.size() << ": Burst = " << burstTime << ", Arrival = " << arrivalTime << endl;
    }

    // Step 2: Sort processes by burst time, arrival time, and process ID
    auto vec = getSorted(burstTimes, arrivalTimes);

    // Step 3: Compute waiting and turnaround times using the sorted order
    auto waiting = getWaitingTimes(vec);
    auto tat = getTurnaroundTimes(vec, waiting);

    // Step 4: Print processes in original input order
    cout << "\nProcesses in original input order:\n";
    for (int i = 0; i < burstTimes.size(); i++) {
        int processId = i + 1;
        auto it = find_if(vec.begin(), vec.end(), [processId](const auto& t) {
            return get<0>(t) == processId;
        });
        int waitingTime = waiting[it - vec.begin()];
        int turnaroundTime = tat[it - vec.begin()];
        cout << "Process " << processId
             << " -> Burst: " << burstTimes[i]
             << " | Arrival: " << arrivalTimes[i]
             << " | Waiting: " << waitingTime
             << " | Turnaround: " << turnaroundTime << endl;
    }

    // Step 5: Print Gantt chart (shows execution timeline)
    cout << "\nGantt Chart:\n";
    cout << "0";  // always start at time 0
    int cumulative = 0;
    for (int i = 0; i < vec.size(); i++) {
        cumulative = max(cumulative, get<2>(vec[i])) + get<1>(vec[i]); // add burst time
        cout << " " << cumulative;   // mark completion time
    }
    cout << endl;

    // Step 6: Compute and print averages
    double avgWait = 0, avgTAT = 0;
    for (int i = 0; i < vec.size(); i++) {
        avgWait += waiting[i];
        avgTAT += tat[i];
    }
    cout << "\nAverage waiting time: " << avgWait / vec.size() << endl;
    cout << "Average turnaround time: " << avgTAT / vec.size() << endl;
}

// ==============================
// MAIN FUNCTION
// ==============================
int main() {
    vector<int> burstTimes; // stores user-entered burst times
    vector<int> arrivalTimes; // stores user-entered arrival times

    // Start measuring execution time
    auto start = chrono::high_resolution_clock::now();

    // Call main procedure
    proc(burstTimes, arrivalTimes);

    // Stop measuring execution time
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    // Print how long program took to execute
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
