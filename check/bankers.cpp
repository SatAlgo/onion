#include <iostream>
using namespace std;

const int MAXP = 10, MAXR = 10;

bool isSafe(int n, int m, int alloc[MAXP][MAXR], int maxReq[MAXP][MAXR], int avail[MAXR]) {
    int work[MAXR], finish[MAXP] = {0}, seq[MAXP], idx = 0;
    for (int j = 0; j < m; ++j) 
    work[j] = avail[j];
//Because we need to stop when:
//   ✔ All processes are finished (done == n)
        for (int done = 0; done < n;) {
        bool progressed = false;  //it tells about Did ANY process finish in this round
           for (int p = 0; p < n; ++p){
            if (!finish[p]) {
            bool ok = true; //ok tells about Can this process finish right now
         
            for (int r = 0; r < m; ++r){
                if (maxReq[p][r] - alloc[p][r] > work[r]) // need[p][r]>work[r] then ok=false and skip that resource
                  {  ok = false; break;
                    }
            }
                 if (ok) {// ok=true add allocation to the work that is available
                for (int r = 0; r < m; ++r) {
                work[r] += alloc[p][r];
                }
                finish[p] = 1;
                 seq[idx++] = p; 
                ++done; //Counts how many processes have finished
                progressed = true;  //something has finished i.e done++ so progressed =true
            }
        }
    }
        if (!progressed) return false;
    }
 cout << "Safe sequence: ";
    for (int i = 0; i < n; ++i) cout << "P" << seq[i] << (i+1==n? "\n":" -> ");
    return true;
}
bool requestResources(int pid, int n, int m, int request[MAXR],int alloc[MAXP][MAXR], int maxReq[MAXP][MAXR], int avail[MAXR]) {
    // 1) Check request <= need
    for (int r = 0; r < m; ++r) {
        int need = maxReq[pid][r] - alloc[pid][r];
        if (request[r] > need)
         { cout<<"Error: Request exceeds declared maximum need.\n"; return false; }
    }
    // 2) Check request <= avail
    for (int r = 0; r < m; ++r) if (request[r] > avail[r]) { cout<<"Resources not available now.\n"; return false; }

    // 3) Try temporarily allocate
    for (int r = 0; r < m; ++r) { avail[r] -= request[r]; alloc[pid][r] += request[r]; }

    // 4) Safety check
    if (isSafe(n,m,alloc,maxReq,avail)) {
        cout << "Request GRANTED for P" << pid << ".\n";
        return true;
    }
 // 5) Rollback
    for (int r = 0; r < m; ++r) 
    { avail[r] += request[r]; alloc[pid][r] -= request[r]; }
    cout << "Request DENIED (would lead to unsafe state).\n";
    return false;
}

int main() {
    int n, m; 
    cout << "Processes? ";
     cin >> n;
    cout << "Resource types? "; 
    cin >> m;
    int alloc[MAXP][MAXR], maxReq[MAXP][MAXR], avail[MAXR];

    cout << "Enter Allocation matrix ("<<n<<"x"<<m<<"):\n";
    for (int i=0;i<n;++i) 
    for (int j=0;j<m;++j) 
    cin>>alloc[i][j];

    cout << "Enter Maximum matrix ("<<n<<"x"<<m<<"):\n";
    for (int i=0;i<n;++i)
     for (int j=0;j<m;++j) 
     cin>>maxReq[i][j];

    cout << "Enter Available resources ("<<m<<"):\n";
    for (int j=0;j<m;++j)
     cin>>avail[j];

    cout << "\nInitial safety check:\n";
    if (!isSafe(n,m,alloc,maxReq,avail))
     { cout << "System is NOT safe initially.\n";
         return 0; }
    else cout << "System is initially SAFE.\n";

    // Ask user for a request
    int pid;
     cout << "\nEnter process id making request (0.."<<n-1<<", -1 to exit): ";
    while (cin >> pid && pid != -1) {
        if (pid < 0 || pid >= n)
         { cout << "Invalid pid. Try again: "; 
            continue; }
        int req[MAXR];
        cout << "Enter request vector ("<<m<<") for P" << pid << ":\n";
        for (int j=0;j<m;++j)
         cin>>req[j];

        requestResources(pid, n, m, req, alloc, maxReq, avail);

        cout << "\nCurrent Available: ";
        for (int j=0;j<m;++j) cout<<avail[j]<<(j+1==m? "\n":" ");
        cout << "\nNext pid (-1 to exit): ";
    }
     cout << "Exiting.\n";
    return 0;
}



bankers
