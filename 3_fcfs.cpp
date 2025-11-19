#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;


void fcfs_scheduling_simple() {
  
    vector<int> pid = {1, 2, 3, 4};
    vector<int> arrival = {0, 2, 4, 6};
    vector<int> burst = {7, 4, 1, 4};
    int n = pid.size();

    
    vector<tuple<int, int, int>> processes;

    for (int i = 0; i < n; ++i) {
        processes.emplace_back(arrival[i], burst[i], i);
    }

        sort(processes.begin(), processes.end());

    
    vector<int> completion(n);
    vector<int> turnaround(n);
    vector<int> waiting(n);
    
    int current_time = 0;
    double total_wait = 0;
    double total_turnaround = 0;

  
    for (const auto& p : processes) {
        int arr_time = get<0>(p);
        int bus_time = get<1>(p);
        int original_index = get<2>(p);
        
        if (current_time < arr_time) {
            current_time = arr_time;
        }
        
        
        completion[original_index] = current_time + bus_time;
        current_time = completion[original_index]; // Update time for next job

        
        turnaround[original_index] = completion[original_index] - arr_time;

        waiting[original_index] = turnaround[original_index] - bus_time;
        
        total_wait += waiting[original_index];
        total_turnaround += turnaround[original_index];
    }

    cout << "\n--- FCFS Scheduling Results (First-Come, First-Served) ---" << endl;
    cout << left << setw(5) << "PID" << setw(10) << "Arrival" << setw(8) << "Burst"
         << setw(12) << "Completion" << setw(12) << "Turnaround" << setw(8) << "Wait" << endl;
    cout << "---------------------------------------------------------" << endl;

    for (int i = 0; i < n; ++i) {
        cout << left << setw(5) << pid[i] << setw(10) << arrival[i] << setw(8) << burst[i]
             << setw(12) << completion[i] << setw(12) << turnaround[i] << setw(8) << waiting[i] << endl;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << total_wait / n << endl;
    cout << "Average Turnaround Time: " << total_turnaround / n << endl;
}

int main() {
    fcfs_scheduling_simple();
    return 0;
}
