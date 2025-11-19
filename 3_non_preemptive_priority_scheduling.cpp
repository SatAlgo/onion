#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

void priority_scheduling_simple() {
   
    vector<int> pid = {1, 2, 3, 4};
    vector<int> arrival = {0, 1, 2, 4};
    vector<int> burst = {6, 8, 7, 3};
    vector<int> priority = {2, 4, 1, 3};
    int n = pid.size();

    
    vector<int> completion(n);
    vector<int> turnaround(n);
    vector<int> waiting(n);
    vector<bool> is_completed(n, false);

    int current_time = 0;
    int processes_completed = 0;
    double total_wait = 0;
    double total_turnaround = 0;

    while (processes_completed < n) {
        int highest_priority = numeric_limits<int>::max();
        int best_index = -1;

        for (int i = 0; i < n; ++i) {
        
            if (arrival[i] <= current_time && !is_completed[i]) {
                
                if (priority[i] < highest_priority) {
                    highest_priority = priority[i];
                    best_index = i;
                }
                
                else if (priority[i] == highest_priority) {
                    
                    if (best_index == -1 || arrival[i] < arrival[best_index]) {
                         best_index = i;
                    }
                }
            }
        }

        
        if (best_index == -1) {
            current_time++;
            continue; 
        }

      
        int i = best_index;

        completion[i] = current_time + burst[i];
        turnaround[i] = completion[i] - arrival[i];
        waiting[i] = turnaround[i] - burst[i];


        is_completed[i] = true;
        current_time = completion[i];
        processes_completed++;

  
        total_wait += waiting[i];
        total_turnaround += turnaround[i];
    }

    cout << "\n--- Priority Scheduling Results (Non-Preemptive) ---" << endl;
    cout << "(Note: Lower Priority Number = Higher Priority)" << endl;
    cout << left << setw(5) << "PID" << setw(10) << "Arrival" << setw(8) << "Burst"
         << setw(9) << "Priority" << setw(12) << "Completion" << setw(12) << "Turnaround" << setw(8) << "Wait" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (int i = 0; i < n; ++i) {
        cout << left << setw(5) << pid[i] << setw(10) << arrival[i] << setw(8) << burst[i]
             << setw(9) << priority[i] << setw(12) << completion[i] << setw(12) << turnaround[i] << setw(8) << waiting[i] << endl;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << total_wait / n << endl;
    cout << "Average Turnaround Time: " << total_turnaround / n << endl;
}

int main() {
    priority_scheduling_simple();
    return 0;
}
