#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

const int MAX_BUFFER_SIZE = 5;
vector<int> buffer;
int item_counter = 0;


mutex mtx;
condition_variable cv_producer;
condition_variable cv_consumer;

void producer() {
    for (int i = 0; i < 10; ++i) {
        unique_lock<mutex> lock(mtx);

        cv_producer.wait(lock, []{ return buffer.size() < MAX_BUFFER_SIZE; });

        int produced_item = ++item_counter;
        buffer.push_back(produced_item);
        
        cout << "P: Produced item " << produced_item << ". Buffer size: " << buffer.size() << endl;

        lock.unlock();

        cv_consumer.notify_one(); 

        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void consumer() {
    for (int i = 0; i < 10; ++i) {
        unique_lock<mutex> lock(mtx);
        
        cv_consumer.wait(lock, []{ return !buffer.empty(); });

        int consumed_item = buffer.back();
        buffer.pop_back();

        cout << "C: Consumed item " << consumed_item << ". Buffer size: " << buffer.size() << endl;

        lock.unlock();

        cv_producer.notify_one();

        
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    cout << "--- Producer-Consumer Synchronization Demo ---" << endl;
    cout << "Buffer Max Size: " << MAX_BUFFER_SIZE << endl;
    cout << "Goal: Produce and consume 10 items safely." << endl;
    
    
    thread producer_thread(producer);
    thread consumer_thread(consumer);

    
    producer_thread.join();
    consumer_thread.join();

    cout << "\n--- Demo Finished ---" << endl;
    cout << "Final item count produced: " << item_counter << endl;
    cout << "Final buffer size: " << buffer.size() << endl;

    return 0;
}
