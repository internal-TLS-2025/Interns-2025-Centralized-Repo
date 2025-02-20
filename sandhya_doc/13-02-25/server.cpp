#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 1234;
const int BUFFER_SIZE = 1024;

int main() {
    int server_fd, new_socket;
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Define the address of the server
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Bind to all interfaces
    address.sin_port = htons(PORT); // Set port number

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Read and echo messages
    while (true) {
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        std::cout << "Received: " << buffer << std::endl;
        send(new_socket, buffer, valread, 0); // Echo back to client
        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
