#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <unordered_set>
#include <climits>
using namespace std;

// ==============================
// Function: scheduleSJF
// Purpose: Run non-preemptive SJF with arrival times
// Returns waiting times, turnaround times, and Gantt chart order
// ==============================
void scheduleSJF(const vector<int>& burstTimes,
                 const vector<int>& arrivalTimes,
                 vector<int>& waiting,
                 vector<int>& turnaround,
                 vector<int>& ganttOrder) 
{
    int n = burstTimes.size();
    waiting.assign(n, 0);
    turnaround.assign(n, 0);
    vector<bool> done(n, false);

    int currentTime = 0, completed = 0;

    while (completed < n) {
        int idx = -1;
        int shortest = INT_MAX;

        // Pick shortest available process
        for (int i = 0; i < n; i++) {
            if (!done[i] && arrivalTimes[i] <= currentTime) {
                if (burstTimes[i] < shortest || 
                   (burstTimes[i] == shortest && i < idx)) {
                    shortest = burstTimes[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // Jump to next arrival if nothing ready
            int nextArrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!done[i]) nextArrival = min(nextArrival, arrivalTimes[i]);
            }
            currentTime = nextArrival;
            continue;
        }

        // Run process
        currentTime += burstTimes[idx];
        ganttOrder.push_back(idx + 1);

        // Compute WT and TAT directly from Gantt
        turnaround[idx] = currentTime - arrivalTimes[idx];
        waiting[idx] = turnaround[idx] - burstTimes[idx];

        done[idx] = true;
        completed++;
    }
}

// ==============================
// Function: proc
// ==============================
void proc(vector<int>& burstTimes, vector<int>& arrivalTimes) {
    int burstTime, arrivalTime;
    unordered_set<int> arrivalSet;

    cout << "Enter burst times and arrival times (0 0 to stop):" << endl;
    while (cin >> burstTime >> arrivalTime && (burstTime != 0 || arrivalTime != 0)) {
        if (arrivalSet.find(arrivalTime) != arrivalSet.end()) {
            cout << "Error: Duplicate arrival time detected. Please enter a unique arrival time." << endl;
            continue;
        }
        burstTimes.push_back(burstTime);
        arrivalTimes.push_back(arrivalTime);
        arrivalSet.insert(arrivalTime);
        cout << "Process " << burstTimes.size() << ": Burst = " << burstTime << ", Arrival = " << arrivalTime << endl;
    }

    vector<int> waiting, turnaround, ganttOrder, completion;
    scheduleSJF(burstTimes, arrivalTimes, waiting, turnaround, ganttOrder);

    // Print per-process results
    cout << "\nProcesses:\n";
    for (int i = 0; i < burstTimes.size(); i++) {
        cout << "Process " << i+1
             << " -> Burst: " << burstTimes[i]
             << " | Arrival: " << arrivalTimes[i]
             << " | Waiting: " << waiting[i]
             << " | Turnaround: " << turnaround[i] << endl;
    }

    // Print Gantt chart
    cout << "\nGantt Chart:\n0";
    int currentTime = 0;
    for (int pid : ganttOrder) {
        int idx = pid - 1;
        currentTime += burstTimes[idx];
        cout << " " << currentTime;
    }
    cout << endl;

    // Averages
    double avgWait = 0, avgTat = 0;
    for (int i = 0; i < burstTimes.size(); i++) {
        avgWait += waiting[i];
        avgTat += turnaround[i];
    }
    cout << "\nAverage waiting time: " << avgWait / burstTimes.size() << endl;
    cout << "Average turnaround time: " << avgTat / burstTimes.size() << endl;
}

// ==============================
// MAIN
// ==============================
int main() {
    vector<int> burstTimes, arrivalTimes;

    auto start = chrono::high_resolution_clock::now();
    proc(burstTimes, arrivalTimes);
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    return 0;
}
