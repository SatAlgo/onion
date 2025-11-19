#include <iostream>
#include <unistd.h> 
#include <sys/wait.h> 
#include <cstring> 

using namespace std;

void run_pipe_demo() {
    int pipefd[2];
    pid_t pid;
    const char *message = "Parent sends a short message via pipe.";
    char buffer[100];

    if (pipe(pipefd) == -1) {
        cerr << "Error: Pipe creation failed." << endl;
        return;
    }

    pid = fork();

    if (pid < 0) {
        cerr << "Error: Fork failed." << endl;
        return;
    }

    if (pid > 0) { 
        // --- Parent Process (Writer) ---
        
        // 1. Close read end (pipefd[0])
        close(pipefd[0]);

        // 2. Write data and close write end (pipefd[1])
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);

        cout << "Parent (PID " << getpid() << "): Sent data and waiting for child." << endl;

        // 3. Wait for the child to finish
        wait(NULL);

    } else { 
        // --- Child Process (Reader) ---
        
        // 1. Close write end (pipefd[1])
        close(pipefd[1]);

        // 2. Read data (blocks until parent writes)
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        cout << "Child (PID " << getpid() << "): Received message: '" << buffer << "'" << endl;
        
        _exit(0);
    }
}

int main() {
    run_pipe_demo();
    return 0;
}
