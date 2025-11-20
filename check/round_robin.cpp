#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 20;

int main() {
    int n, tq;
    cout<<"Enter number of processes: ";
    cin>>n;
    cout<<"Enter Time Quantum: ";
    cin>>tq;

    int pid[MAX], at[MAX], bt[MAX], rem[MAX], ct[MAX]={0}, tat[MAX]={0}, wt[MAX]={0};
    
    // Input
    for(int i=0;i<n;i++){
        pid[i] = i + 1;
        cout<<"P"<<pid[i]<<" AT BT: ";
        cin>>at[i]>>bt[i];
        rem[i] = bt[i];        // remaining burst time
    }

    int t = 0, finished = 0;

    // Round Robin execution
    while(finished < n){
        bool flag = false;    // to check if any process was executed in this round
          for(int i=0;i<n;i++){
            if(rem[i] > 0 && at[i] <= t){
                flag = true;
                // if process can finish in this time slice
                if(rem[i] <= tq){
                    t+= rem[i];
                    rem[i] = 0;
                    ct[i] = t;
                    tat[i] = ct[i] - at[i];
                    wt[i] = tat[i] - bt[i];
                    finished++;
                }
                else{
                    rem[i] -= tq;
                    t += tq;
                }
            }
        }
        if(!flag) t++; // if CPU idle
    }

    // Output
    cout<<"\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    double sumT=0, sumW=0;

    for(int i=0;i<n;i++){
        cout<<pid[i]<<'\t'<<at[i]<<'\t'<<bt[i]<<'\t'<<ct[i]<<'\t'<<tat[i]<<'\t'<<wt[i]<<'\n';
        sumT += tat[i];
        sumW += wt[i];
    }

    cout<<fixed<<setprecision(2);
    cout<<"\nAverage TAT = "<<sumT/n<<"\nAverage WT = "<<sumW/n<<"\n";

    return 0;
}


roundrobin
