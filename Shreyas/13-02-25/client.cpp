#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
 
#define PORT 55555
#define BUFFER_SIZE 1024
 
int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};
 
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }
 
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
 
    // Convert IPv4 address from text to binary
if (inet_pton(AF_INET, "10.113.53.85", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }
 
    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection to server failed" << std::endl;
        return -1;
    }
 
    std::cout << "Connected to the server. Type 'exit' to quit.\n";
 
    while (true) {
        std::cout << "You: ";
        std::string message;
        std::getline(std::cin, message);
 
        // Exit condition
        if (message == "exit") {
            break;
        }
 
        // Send message to server
        send(sock, message.c_str(), message.length(), 0);
 
        // Receive response from server
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            buffer[valread] = '\0';  // Null-terminate the received message
            std::cout << "Server: " << buffer << std::endl;
        } else {
            std::cerr << "Server disconnected" << std::endl;
            break;
        }
 
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    }
 
    // Close socket
    close(sock);
    std::cout << "Disconnected from server.\n";
 
    return 0;
}
