#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>

using namespace std;


bool is_safe(
    int num_processes, 
    int num_resources, 
    const vector<vector<int>>& alloc,
    const vector<vector<int>>& need,
    vector<int> available
) {
    
    vector<bool> finish(num_processes, false);
    vector<int> safe_sequence(num_processes);
    int count = 0;

    while (count < num_processes) {
        bool found_process = false;

        for (int i = 0; i < num_processes; ++i) {
    
            if (finish[i] == false) {
                bool can_run = true;
                
                
                for (int r = 0; r < num_resources; ++r) {
                    if (need[i][r] > available[r]) {
                        can_run = false;
                        break;
                    }
                }

                
                if (can_run) {
                    
                    for (int r = 0; r < num_resources; ++r) {
                        available[r] += alloc[i][r];
                    }
                    
                    
                    finish[i] = true;
                    safe_sequence[count] = i;
                    count++;
                    found_process = true;
                    break;
                }
            }
        }


        if (!found_process) {
            cout << "\nSYSTEM STATE: UNSAFE (Cannot find a safe sequence)." << endl;
            return false;
        }
    }

    
    cout << "\nSYSTEM STATE: SAFE" << endl;
    cout << "Safe Sequence: <";
    for (int i = 0; i < num_processes; ++i) {
        cout << "P" << safe_sequence[i];
        if (i < num_processes - 1) cout << ", ";
    }
    cout << ">" << endl;
    
    return true;
}

int main() {
    
    const int P = 5;
    const int R = 3;

    
    vector<vector<int>> allocation = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2} 
    };

    
    vector<vector<int>> max_needs = {
        {7, 5, 3},
        {3, 2, 2}, 
        {9, 0, 2}, 
        {2, 2, 2}, 
        {4, 3, 3} 
    };

    
    vector<int> available = {3, 3, 2};
    
    
    vector<vector<int>> need(P, vector<int>(R));
    for (int i = 0; i < P; ++i) {
        for (int r = 0; r < R; ++r) {
            need[i][r] = max_needs[i][r] - allocation[i][r];
        }
    }
    
    cout << "--- Banker's Algorithm: Safety Check ---" << endl;
    cout << "Processes: " << P << ", Resources: " << R << endl;
    cout << "Initial Available: A=" << available[0] << ", B=" << available[1] << ", C=" << available[2] << endl;

    
    cout << "\nNeed Matrix:" << endl;
    for (int i = 0; i < P; ++i) {
        cout << "P" << i << ": (" << need[i][0] << ", " << need[i][1] << ", " << need[i][2] << ")" << endl;
    }

    is_safe(P, R, allocation, need, available);

    return 0;
}
