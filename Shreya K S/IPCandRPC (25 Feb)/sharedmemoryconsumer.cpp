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
    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    // Map shared memory object into address space
    void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Read the message from shared memory
    std::cout << "Consumer: Message from shared memory: " << (char *)ptr << std::endl;

    // Cleanup
    munmap(ptr, SHM_SIZE);
    close(shm_fd);

    return 0;
}
