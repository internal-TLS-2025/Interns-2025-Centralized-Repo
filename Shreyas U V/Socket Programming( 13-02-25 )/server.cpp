#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 55555
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Set socket options failed" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        return -1;
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listening failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accepting a connection from a client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accepting connection failed" << std::endl;
        return -1;
    }

    // Receiving and echoing back messages
    while (true) {
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cerr << "Client disconnected or error" << std::endl;
            break;
        }
        std::cout << "Received: " << buffer << std::endl;
        send(new_socket, buffer, valread, 0); // Echo back the message
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
