#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

const char *shm_name = "/my_shared_mem";
const int SHM_SIZE = 4096;  // Size of shared memory

int main() {
    // Open shared memory object
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    // Set the size of the shared memory object
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    // Map shared memory object into address space
    void *ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Write a message to the shared memory
    const char *message = "Hello from the producer!";
    memcpy(ptr, message, strlen(message) + 1);  // Include the null terminator

    std::cout << "Producer: Message written to shared memory.\n";

    // Cleanup
    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;
}
