#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

void allocate_memory(
    vector<int> initial_blocks, 
    const vector<int>& process_sizes, 
    const string& algorithm_name,
    function<int(const vector<int>&, int)> find_best_fit_index
) {
    vector<int> blocks = initial_blocks;
    int num_processes = process_sizes.size();
    
    vector<int> allocation_block_index(num_processes, -1);
    int total_internal_fragmentation = 0;
    int total_allocated_processes = 0;

    cout << "\n--- " << algorithm_name << " Algorithm ---" << endl;
    cout << "Initial Blocks: ";
    for (int size : initial_blocks) {
        cout << size << " ";
    }
    cout << endl;
    cout << left << setw(10) << "Process" << setw(10) << "Size" << setw(10) << "Block ID" << setw(15) << "Fragment" << endl;
    cout << "--------------------------------------------" << endl;

    for (int p = 0; p < num_processes; ++p) {
        int process_size = process_sizes[p];

        int block_index = find_best_fit_index(blocks, process_size);

        cout << left << setw(10) << p + 1 << setw(10) << process_size;

        if (block_index != -1) {
            
            int fragment = blocks[block_index] - process_size;
            total_internal_fragmentation += fragment;
            total_allocated_processes++;

            allocation_block_index[p] = block_index + 1;

            blocks[block_index] -= process_size; 

            cout << setw(10) << block_index + 1 << setw(15) << fragment << endl;
        } else {
            cout << setw(10) << "N/A" << setw(15) << "N/A (Failed)" << endl;
        }
    }

    cout << "--------------------------------------------" << endl;
    cout << "Total Allocated: " << total_allocated_processes << " / " << num_processes << endl;
    cout << "Total Internal Fragmentation: " << total_internal_fragmentation << " units" << endl;
    cout << "Final Block Status: ";
    for (int size : blocks) {
        cout << size << " ";
    }
    cout << endl;
}

int find_first_fit(const vector<int>& blocks, int process_size) {
    for (int i = 0; i < blocks.size(); ++i) {
        if (blocks[i] >= process_size) {
            return i;
        }
    }
    return -1;
}

int find_best_fit(const vector<int>& blocks, int process_size) {
    int best_index = -1;
    int min_fragment = numeric_limits<int>::max();

    for (int i = 0; i < blocks.size(); ++i) {
        if (blocks[i] >= process_size) {
            int current_fragment = blocks[i] - process_size;
            if (current_fragment < min_fragment) {
                min_fragment = current_fragment;
                best_index = i;
            }
        }
    }
    return best_index;
}

int find_worst_fit(const vector<int>& blocks, int process_size) {
    int worst_index = -1;
    int max_block_size = -1;

    for (int i = 0; i < blocks.size(); ++i) {
        if (blocks[i] >= process_size) {
            if (blocks[i] > max_block_size) {
                max_block_size = blocks[i];
                worst_index = i;
            }
        }
    }
    return worst_index;
}


int main() {
    vector<int> memory_blocks = {100, 500, 200, 300, 600};
    vector<int> process_requests = {212, 417, 112, 426};

    cout << "Memory Allocation Simulation" << endl;
    cout << "============================" << endl;

    allocate_memory(memory_blocks, process_requests, "First Fit", find_first_fit);

    allocate_memory(memory_blocks, process_requests, "Best Fit", find_best_fit);
    
    allocate_memory(memory_blocks, process_requests, "Worst Fit", find_worst_fit);

    return 0;
}
