#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <cstring>
#include <cstdlib>

using namespace std;

// Define common constants
#define MSG_KEY 0x5678
#define MAX_MSG_TEXT_SIZE 256
#define MSG_TYPE 1

// Structure required for System V Message Queue
struct Message {
    long message_type; // MUST be the first member
    char message_text[MAX_MSG_TEXT_SIZE];
};

void run_message_queue_demo() {
    int msqid;
    pid_t pid;
    
    // 1. Get/Create the Message Queue
    msqid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msqid < 0) {
        perror("msgget failed");
        return;
    }

    // 2. Create the Child Process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        msgctl(msqid, IPC_RMID, NULL); // Cleanup on fork failure
        return;
    }

    // --- PARENT PROCESS (The Sender) ---
    if (pid > 0) {
        Message message_to_send;
        message_to_send.message_type = MSG_TYPE;
        const char* msg_content = "The Message Queue provides asynchronous communication.";
        strcpy(message_to_send.message_text, msg_content);

        cout << "PARENT (PID " << getpid() << "): Sending message..." << endl;

        // msgsnd: Send the message payload size (excluding the long message_type)
        size_t message_size = strlen(message_to_send.message_text) + 1;
        
        if (msgsnd(msqid, &message_to_send, message_size, 0) == -1) {
            perror("msgsnd failed");
        } else {
            cout << "PARENT: Message sent successfully." << endl;
        }

        // Wait for the child process to terminate
        wait(NULL);
        
        // 4. Remove the Queue (Cleanup)
        msgctl(msqid, IPC_RMID, NULL);
        cout << "PARENT: Message Queue deleted. Exiting." << endl;
    }
    
    // --- CHILD PROCESS (The Receiver) ---
    else {
        Message received_message;
        
        cout << "CHILD (PID " << getpid() << "): Waiting for message of type " << MSG_TYPE << "..." << endl;
        
        // msgrcv: Receive message of the specific type (MSG_TYPE)
        ssize_t bytes_read = msgrcv(msqid, &received_message, MAX_MSG_TEXT_SIZE, MSG_TYPE, 0);

        if (bytes_read > 0) {
            cout << "CHILD: Received message: \"" << received_message.message_text << "\"" << endl;
        } else if (bytes_read == -1) {
            perror("msgrcv failed");
        }
        
        exit(0);
    }
}

int main() {
    run_message_queue_demo();
    return 0;
}
