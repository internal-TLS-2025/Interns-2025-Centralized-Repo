#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    const char* fifoPath = "my_fifo";
    char buffer[100];

    int fd = open(fifoPath, O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    
    std::cout << "Received message: " << buffer << std::endl;
    close(fd);
    
    return 0;
}
