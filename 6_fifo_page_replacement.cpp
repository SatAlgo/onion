#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

void run_fifo_simulation(const vector<int>& page_references, int num_frames) {
    vector<int> frames; 
    queue<int> fifo_queue; 

    int page_faults = 0;
    int n = page_references.size();

    cout << "\n--- FIFO Page Replacement Simulation ---" << endl;
    cout << "Total References: " << n << ", Available Frames: " << num_frames << endl;
    cout << "--------------------------------------------------------" << endl;

    cout << left << setw(10) << "Reference" << " | ";
    for (int i = 0; i < num_frames; ++i) {
        cout << setw(5) << "Frame" + to_string(i+1);
    }
    cout << " | " << "Fault?" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int page : page_references) {
        bool page_hit = (find(frames.begin(), frames.end(), page) != frames.end());

        cout << left << setw(10) << page << " | ";

        if (!page_hit) {
            page_faults++;
            
            if (frames.size() < num_frames) {
                frames.push_back(page);
                fifo_queue.push(page);
                
            } else {
                int page_to_replace = fifo_queue.front();
                fifo_queue.pop();

                auto it = find(frames.begin(), frames.end(), page_to_replace);
                if (it != frames.end()) {
                    *it = page;
                }
                
                fifo_queue.push(page);
            }
        }

        for (int frame_page : frames) {
            cout << setw(5) << frame_page;
        }
        for (size_t i = frames.size(); i < num_frames; ++i) {
            cout << setw(5) << "-";
        }

        cout << " | " << (page_hit ? "NO" : "YES") << endl;
    }

    cout << "--------------------------------------------------------" << endl;
    cout << "Final Total Page Faults: " << page_faults << endl;
    cout << "Total Hits: " << n - page_faults << endl;
}

int main() {
    vector<int> page_references = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int num_frames = 3; 

    run_fifo_simulation(page_references, num_frames);

    return 0;
}
