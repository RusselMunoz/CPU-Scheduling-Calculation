#include <iostream>
#include <vector>
using namespace std;


int main() {
    vector<int> burstTimes;
    vector<int> arrivalTimes;

    int burstTime, arrivalTime;

    while ( cin >> burstTime >> arrivalTime && (burstTime != 0 || arrivalTime != 0)) {
        cout << "burstTime: " << burstTime << ", arrivalTime: " << arrivalTime << endl;
    }
    burstTimes.push_back(burstTime);
    arrivalTimes.push_back(arrivalTime);

    for (size_t i = 0; i < burstTimes.size(); ++i) {
        cout << "Process " << i + 1 << ": Burst Time = " << burstTimes[i] << ", Arrival Time = " << arrivalTimes[i] << endl;
        }
        return 0;
}

