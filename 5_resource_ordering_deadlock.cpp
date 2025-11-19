#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

using namespace std;

mutex R1_Low_Order;
mutex R2_High_Order;

void safe_process(int id) {
    cout << "Process P" << id << ": Starting..." << endl;

    cout << "Process P" << id << ": Requesting R1 (Order 1)..." << endl;
    
    unique_lock<mutex> lock1(R1_Low_Order);
    
    cout << "Process P" << id << ": ACQUIRED R1. Holding it..." << endl;
    this_thread::sleep_for(chrono::milliseconds(200));

    cout << "Process P" << id << ": Requesting R2 (Order 2)..." << endl;
    
    unique_lock<mutex> lock2(R2_High_Order);
    
    cout << "Process P" << id << ": ACQUIRED R2. Critical section running." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Process P" << id << ": Finished and released R1 and R2." << endl;
}

void unsafe_process_A() {
}
void unsafe_process_B() {
}


int main() {
    cout << "--- Resource Ordering Deadlock Prevention Demo ---" << endl;
    cout << "Goal: Prevent Circular Wait by enforcing R1 request BEFORE R2 request." << endl;
    
    thread P1(safe_process, 1);
    thread P2(safe_process, 2);

    P1.join();
    P2.join();

    cout << "\nRESULT: Deadlock was successfully prevented." << endl;
    cout << "The threads ran sequentially (P1 finished, then P2 started, or vice-versa) "
         << "or concurrently, but never blocked each other indefinitely." << endl;

    return 0;
}
