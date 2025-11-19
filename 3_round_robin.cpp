#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

void round_robin_scheduling_simple() {
    
    vector<int> pid = {1, 2, 3};
    vector<int> arrival = {0, 1, 2};
    vector<int> burst = {10, 5, 8};
    int n = pid.size();
    int quantum = 2;

    
    vector<int> remaining(burst);
    queue<int> ready_queue;
    vector<bool> in_queue(n, false);

    
    vector<int> completion(n);
    vector<int> turnaround(n);
    vector<int> waiting(n);
    
    int current_time = 0;
    int processes_completed = 0;
    int process_index = 0;
    double total_wait = 0;
    double total_turnaround = 0;

    
    for(int i = 0; i < n; ++i) {
        if (arrival[i] <= 0) {
            ready_queue.push(i);
            in_queue[i] = true;
            process_index++;
        }
    }

  
    while (processes_completed < n) {
        if (ready_queue.empty()) {
            current_time++;
            
            while (process_index < n && arrival[process_index] <= current_time) {
                ready_queue.push(process_index);
                in_queue[process_index] = true;
                process_index++;
            }
            continue;
        }

        int i = ready_queue.front();
        ready_queue.pop();
        in_queue[i] = false;

        
        int execution_time = min(quantum, remaining[i]);
        
        current_time += execution_time;
        remaining[i] -= execution_time;

        
        while (process_index < n && arrival[process_index] <= current_time) {
            if (!in_queue[process_index]) {
                ready_queue.push(process_index);
                in_queue[process_index] = true;
            }
            process_index++;
        }

        if (remaining[i] == 0) {
            
            completion[i] = current_time;
            turnaround[i] = completion[i] - arrival[i];
            waiting[i] = turnaround[i] - burst[i];
            processes_completed++;

            total_wait += waiting[i];
            total_turnaround += turnaround[i];
        } else {
            
            ready_queue.push(i);
            in_queue[i] = true;
        }
    }

    
    cout << "\n--- Round Robin Scheduling Results (Quantum = " << quantum << ") ---" << endl;
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
    round_robin_scheduling_simple();
    return 0;
}
