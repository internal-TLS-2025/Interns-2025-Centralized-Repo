#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    const char* shm_name = "/my_shm";
    const size_t SIZE = 128;

    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    char* ptr = (char*)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    std::cout << "Received from shared memory: " << ptr << std::endl;

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(shm_name);
    
    return 0;
}
