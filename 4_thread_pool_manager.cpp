#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class ThreadPool {
private:
    mutex queue_mutex;
    condition_variable condition;
    bool stop_flag = false;

    
    vector<thread> workers;
    queue<function<void()>> tasks;

    
    void worker_loop() {
        while (true) {
            function<void()> task;

            { 
                unique_lock<mutex> lock(queue_mutex);

                condition.wait(lock, [this] {
                    return stop_flag || !tasks.empty();
                });

                
                if (stop_flag && tasks.empty()) {
                    return;
                }

                
                task = move(tasks.front());
                tasks.pop();

            }
            
            task(); 
        }
    }

public:
    
    ThreadPool(size_t num_threads) {
        
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(&ThreadPool::worker_loop, this);
            cout << "Main: Launched worker thread " << i << endl;
        }
    }

    ~ThreadPool() {
        { 
            unique_lock<mutex> lock(queue_mutex);
            stop_flag = true;
        } 
        
        
        condition.notify_all();

        
        for (thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        cout << "Main: Thread pool safely shut down." << endl;
    }

    
    template<class F>
    void addTask(F&& task) {
        { 
            unique_lock<mutex> lock(queue_mutex);
            
            tasks.emplace(forward<F>(task));
        }
        
        
        condition.notify_one(); 
    }
};



void execute_task(int id, int duration_ms) {
    cout << "  [Worker " << this_thread::get_id() << "] Starting Task " << id << " for " << duration_ms << "ms." << endl;
    this_thread::sleep_for(chrono::milliseconds(duration_ms));
    cout << "  [Worker " << this_thread::get_id() << "] Finished Task " << id << "." << endl;
}


int main() {
    
    ThreadPool pool(4);
    cout << "--------------------------------------------------------" << endl;

    
    int num_tasks = 15;
    cout << "Main: Adding " << num_tasks << " tasks to the pool queue..." << endl;
    
    for (int i = 1; i <= num_tasks; ++i) {
        
        int duration = (i % 3 == 0) ? 1000 : 300; 
        pool.addTask([i, duration] { 
            execute_task(i, duration); 
        });
    }

    cout << "Main: Finished adding tasks. Waiting for workers to finish..." << endl;
    

    return 0;
}
