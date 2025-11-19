#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <cstring>
#include <cstdlib> // For exit()

using namespace std;

// Define a common key and size for the shared memory segment
#define SHM_KEY 0x1234
#define SHM_SIZE 1024

void run_shared_memory_demo() {
    int shmid;
    pid_t pid;
    char *shared_memory_ptr;
    const char* message = "Shared memory is the fastest IPC mechanism.";

    // 1. Get/Create the Shared Memory Segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return;
    }

    // 2. Attach the Shared Memory Segment
    shared_memory_ptr = (char*)shmat(shmid, NULL, 0);
    if (shared_memory_ptr == (char*)-1) {
        perror("shmat failed");
        shmctl(shmid, IPC_RMID, NULL); // Cleanup segment ID
        return;
    }

    // 3. Create the Child Process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        shmdt(shared_memory_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return;
    }

    // --- PARENT PROCESS (The Writer) ---
    if (pid > 0) {
        cout << "PARENT (PID " << getpid() << "): Writing data..." << endl;

        // Write the message to the shared memory
        memcpy(shared_memory_ptr, message, strlen(message) + 1);

        // Wait for the child to finish
        wait(NULL);

        // 4. Detach and Remove the Segment (Cleanup)
        shmdt(shared_memory_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        cout << "PARENT: Shared Memory cleaned up and removed. Exiting." << endl;
    }
    
    // --- CHILD PROCESS (The Reader) ---
    else {
        // Simple delay is used here for demonstration, but synchronization
        // (like semaphores) is required in a real system.
        sleep(1); 

        cout << "CHILD (PID " << getpid() << "): Reading message: \"" << shared_memory_ptr << "\"" << endl;

        // 4. Detach the Segment
        shmdt(shared_memory_ptr);
        exit(0); // Use exit(0) or _exit(0) for safe child termination
    }
}

int main() {
    run_shared_memory_demo();
    return 0;
}
