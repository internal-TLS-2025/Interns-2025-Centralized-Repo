#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 1024  // Size of the shared memory

int main() {
    // Create shared memory
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    // Attach the shared memory
    char *shared_memory = (char *)shmat(shmid, nullptr, 0);
    if (shared_memory == (char *)-1) {
        perror("shmat");
        return 1;
    }

    // Fork process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {  // Child process (Producer)
        // Write to shared memory
        const char *message = "Hello from the child process!";
        strncpy(shared_memory, message, SHM_SIZE);
        std::cout << "Child process: wrote to shared memory: " << message << std::endl;

        // Simulate some processing time
        sleep(2);
    } else {  // Parent process (Consumer)
        // Wait for the child process to finish writing
        wait(nullptr);

        // Read from shared memory
        std::cout << "Parent process: read from shared memory: " << shared_memory << std::endl;
    }

    // Detach and remove shared memory
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
