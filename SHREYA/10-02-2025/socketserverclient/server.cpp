#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555  // Define the port number
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Optional: Attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "setsockopt error" << std::endl;
        return -1;
    }

    // Setting up the address structure
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address
    address.sin_port = htons(PORT); // Convert port number to network byte order

    // Binding the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen error" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accepting incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept error" << std::endl;
        return -1;
    }

    // Read message from the client
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread < 0) {
        std::cerr << "Read error" << std::endl;
    } else {
        std::cout << "Message from client: " << buffer << std::endl;

        // Respond to the client
        std::string response = "Hello from server!";
        send(new_socket, response.c_str(), response.size(), 0);
    }

    // Clean up
    close(new_socket);
    close(server_fd);

    return 0;
}
